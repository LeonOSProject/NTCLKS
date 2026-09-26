# ntclks standalone kernel build entry point (phase 2 of the kernel/userland
# separation).
#
# GNU Make owns the dependency graph, the parallel schedule and the incremental
# decisions. C helpers under tools/host/ perform data transforms only; short
# scripts under tools/build/ drive third-party builds. Nothing here shells out to
# a second scheduler, and no production path runs Python, Meson or Ninja (the
# Python regression tools under tools/test_*.py run only from `make test`).
#
# Derived from the parent LeonOS 4 build entry point. The product surface here
# is exactly the kernel side: kernel.sys, kernel.debug, loader.elf, the five
# .drv drivers and kerneldebug.sys. Nothing in this tree reads parent or
# product configuration.

# --- GNU Make version ------------------------------------------------------
# Grouped targets ('&:') and the $(file) function both need 4.3.
leonos_make_min := $(shell printf '4.3\n$(MAKE_VERSION)\n' | LC_ALL=C sort -V | head -n1)
ifeq ($(leonos_make_min),4.3)
else
$(error GNU Make >= 4.3 is required, this is $(MAKE_VERSION))
endif

LEONOS_SRC := $(patsubst %/,%,$(dir $(realpath $(firstword $(MAKEFILE_LIST)))))

# --- user-facing variables --------------------------------------------------
# ARCH, PROFILE and O may come from the command line or from these defaults
# only. An inherited environment value is ignored on purpose: an unrelated shell
# setting must not silently change what gets built (plan section 6.2).
ifeq ($(origin ARCH),undefined)
ARCH := x86_64
endif
ifeq ($(origin PROFILE),undefined)
PROFILE := release
endif
ifeq ($(origin O),undefined)
O := $(LEONOS_SRC)/out/$(ARCH)/$(PROFILE)
endif

V ?= 0
SOURCE_DATE_EPOCH ?= $(shell git -C $(LEONOS_SRC) show -s --format=%ct HEAD 2>/dev/null)
TOOLCHAIN ?= $(LEONOS_SRC)/configs/toolchains/llvm-x86_64.mk

O := $(patsubst %/,%,$(O))

# --- input validation -------------------------------------------------------
# The supported character set is enumerated rather than promising arbitrary
# paths: Make word splitting, shell quoting and the generated manifests all break
# on the rejected set, so failing here is far cheaper than failing mid-build.
LEONOS_ALLOWED_CHARS := a b c d e f g h i j k l m n o p q r s t u v w x y z \
	A B C D E F G H I J K L M N O P Q R S T U V W X Y Z \
	0 1 2 3 4 5 6 7 8 9 . _ / -

# $(call strip_allowed,text,chars): keep only characters outside the allow-list.
strip_allowed = $(if $(2),$(call strip_allowed,$(subst $(firstword $(2)),,$(1)),$(wordlist 2,9999,$(2))),$(1))

ifeq ($(O),)
$(error O= must not be empty; it names this build's output directory)
endif
ifneq ($(words $(O)),1)
$(error O='$(O)' is unsupported: whitespace and newlines are not accepted in an output path)
endif
leonos_O_residual := $(call strip_allowed,$(O),$(LEONOS_ALLOWED_CHARS))
ifneq ($(leonos_O_residual),)
$(error O='$(O)' is unsupported: offending characters are [$(leonos_O_residual)]; accepted are A-Z a-z 0-9 . _ / -)
endif
# Compare absolute paths: `O=.` and `O=<src>` are the same refusal. O has already
# been restricted to a safe character set above, so quoting here cannot be escaped.
leonos_O_absolute := $(shell realpath -m -- '$(O)' 2>/dev/null || printf '%s' '$(O)')
ifeq ($(leonos_O_absolute),/)
$(error refusing / as the output directory)
endif
ifeq ($(leonos_O_absolute),$(LEONOS_SRC))
$(error refusing the source root as the output directory (O='$(O)'))
endif
ifneq ($(filter $(ARCH),x86_64),)
else
$(error unsupported ARCH '$(ARCH)'; this build supports ARCH=x86_64)
endif
ifneq ($(filter $(PROFILE),debug release),)
else
$(error unsupported PROFILE '$(PROFILE)'; use PROFILE=debug or PROFILE=release)
endif

# --- output layout ----------------------------------------------------------
O_HOST      := $(O)/host
O_OBJ       := $(O)/obj
O_GENERATED := $(O)/generated
O_INCLUDE   := $(O)/include
O_CONFIG    := $(O)/config
O_LOGS      := $(O)/logs
O_META      := $(O)/meta

# Shared download cache: deliberately outside O because it is profile
# independent and `distclean` must not throw it away. It is a real directory in
# this repository (never a symlink to another checkout).
LEONOS_CACHE := $(LEONOS_SRC)/cache/downloads

# Written when an output tree is created and re-checked before anything is
# deleted, so `clean` can never operate on a directory it does not own.
LEONOS_O_MARKER := $(O)/.leonos-out

# --- same-output-directory mutual exclusion ---------------------------------
# Two top-level makes sharing one O would race on objects, generated headers and
# signature files, so one of them has to be refused outright (plan section 6.3).
# Different O directories are independent and may build concurrently.
#
# The owner is the make process that acquired the lock, and the token names the
# output directory it owns: LEONOS_BUILD_OWNER is "<pid>.<start ticks>:<absolute O>".
# A nested make for that same directory inherits it, so recursive build
# invocations do not refuse their own outer build. A nested make for a different
# directory acquires its own lock, so a test suite that spawns builds still gets
# real exclusion for the trees it creates.
#
# Three cases skip acquisition: dry runs and `make -q` (they promise no output,
# and refusing them would make `make -n` depend on unrelated builds), and goals
# that write nothing at all -- a bare `make` and `make help` must keep working
# while a build is running elsewhere, and must not create the output tree.
leonos_read_only_goals := help
leonos_lock_not_needed :=
ifeq ($(MAKECMDGOALS),)
leonos_lock_not_needed := 1
else ifeq ($(words $(filter $(leonos_read_only_goals),$(MAKECMDGOALS))),$(words $(MAKECMDGOALS)))
leonos_lock_not_needed := 1
endif
leonos_lock_dir := $(O)/.build-lock
leonos_lock_skip := \
	$(if $(findstring n,$(firstword -$(MAKEFLAGS))),1)\
	$(if $(findstring q,$(firstword -$(MAKEFLAGS))),1)\
	$(leonos_lock_not_needed)
ifeq ($(strip $(leonos_lock_skip)),)
LEONOS_BUILD_OWNER := $(shell sh $(LEONOS_SRC)/scripts/build-lock.sh acquire \
	$(leonos_lock_dir) $(leonos_O_absolute))
ifeq ($(LEONOS_BUILD_OWNER),)
$(error refusing to build: '$(O)' is already being built by another make)
endif
export LEONOS_BUILD_OWNER
endif

# --- fragment includes ------------------------------------------------------
# Kernel-side machinery only. The parent's userland/sdk/images fragments (and
# the parse-time components.mk coupling in mk/userland.mk) are deliberately
# absent from this tree.
include $(LEONOS_SRC)/mk/logging.mk
include $(LEONOS_SRC)/mk/host.mk
include $(LEONOS_SRC)/mk/toolchain.mk
include $(LEONOS_SRC)/mk/config.mk
include $(LEONOS_SRC)/mk/kernel.mk
include $(LEONOS_SRC)/mk/headers.mk
include $(LEONOS_SRC)/mk/boot.mk
include $(LEONOS_SRC)/mk/resources.mk

# --- public goals -----------------------------------------------------------
.DEFAULT_GOAL := help
# Source inventories are inputs, never implicit host executable targets.
.SUFFIXES:

.PHONY: help all kernel loader drivers boot tools fetch defconfig olddefconfig \
	menuconfig headers_install install config-sync build-info \
	test test-tools test-abi test-header-export clean distclean

help:
	@printf '%s\n' \
	  'ntclks kernel build (standalone kernel checkout)' \
	  '' \
	  '  all               kernel.sys, kernel.debug, loader.elf, five .drv, kerneldebug.sys' \
	  '  kernel            kernel.sys + kernel.debug' \
	  '  loader            loader.elf (waits for kernel.sys: loader integrity chain)' \
	  '  drivers           mouse.drv serial.drv e1000.drv ac97.drv es1371.drv + kerneldebug.sys' \
	  '  headers_install   export the UAPI whitelist to $(O)/kernel-export/include' \
	  '  install           copy products + manifest.txt to $(DESTDIR)' \
	  '  test              host-tool tests + tools/test_abi_layout.py + tools/test_header_export.py' \
	  '  tools             the host C helpers' \
	  '  fetch             download the locked dependencies into cache/downloads' \
	  '  defconfig / olddefconfig / menuconfig' \
	  '  clean / distclean (distclean also removes the configuration)' \
	  '' \
	  '  variables: O= ARCH= PROFILE=release|debug TOOLCHAIN= SOURCE_DATE_EPOCH= V=1 HOSTCC='

tools: $(LEONOS_HOST_TOOLS)

kernel: $(LEONOS_KERNEL_SYS) $(LEONOS_KERNEL_DEBUG)

# `all` is exactly the six kernel products. There is no userland, no image and
# no package goal in this repository.
all: kernel loader drivers

# Populate the shared download cache from the locked URLs. A normal build never
# downloads: it verifies the cache and stops with this command when bytes are
# missing.
fetch: $(LEONOS_DEPS_TOOL)
	$(Q)sh $(LEONOS_SRC)/tools/build/fetch.sh --deps $(LEONOS_DEPS_TOOL) \
		--lock $(LEONOS_SRC)/configs/dependencies.lock.json --cache $(LEONOS_CACHE) \
		--only unifont

defconfig olddefconfig menuconfig: $(LEONOS_O_MARKER) $(KCONFIG_CONF) $(KCONFIG_MCONF) | $(O_CONFIG)
	$(Q)sh $(LEONOS_SRC)/tools/build/kconfig-frontends.sh run \
		--conf $(abspath $(KCONFIG_CONF)) --mconf $(abspath $(KCONFIG_MCONF)) \
		--kconfig $(KCONFIG_ROOT) --config $(abspath $(LEONOS_CONFIG_FILE)) \
		--seed $(KCONFIG_SEED) --mode $@

config-sync: $(AUTOCONF_H) $(LEONOS_AUTOCONF_MK)

build-info: $(BUILD_INFO_HEADER)

# --- install ---------------------------------------------------------------
# Products plus manifest.txt. The manifest records what was installed and how
# it was built so an installed kernel can be traced back to its inputs: format
# version, arch, kernel git identity (or "no-git"), dirty flag, toolchain
# identity, config and UAPI export digests, the boot handoff version and a
# sha256 per artifact. sha256sum only; no interpreter in the production chain.
DESTDIR ?= $(O)/kernel-install
INSTALL_PRODUCTS := $(LEONOS_KERNEL_SYS) $(LEONOS_KERNEL_DEBUG) $(KERNELDEBUG_SYS) \
	$(LOADER_ELF) $(DRIVER_OUTPUTS)

.PHONY: install
install: $(INSTALL_PRODUCTS) $(HEADER_EXPORT_MANIFEST)
	$(call LEONOS_LOG,INSTALL,$(DESTDIR))
	$(Q)set -eu; \
	dest='$(DESTDIR)'; \
	mkdir -p "$$dest"; \
	cp $(INSTALL_PRODUCTS) "$$dest"/; \
	{ \
	  printf 'format_version: 1\n'; \
	  printf 'arch: %s\n' '$(ARCH)'; \
	  printf 'profile: %s\n' '$(PROFILE)'; \
	  if git -C '$(LEONOS_SRC)' rev-parse --short HEAD >/dev/null 2>&1; then \
	    printf 'kernel_git: %s\n' "$$(git -C '$(LEONOS_SRC)' rev-parse --short HEAD)"; \
	    if [ -n "$$(git -C '$(LEONOS_SRC)' status --porcelain 2>/dev/null)" ]; then \
	      printf 'kernel_git_dirty: yes\n'; \
	    else \
	      printf 'kernel_git_dirty: no\n'; \
	    fi; \
	  else \
	    printf 'kernel_git: no-git\n'; \
	    printf 'kernel_git_dirty: unknown\n'; \
	  fi; \
	  printf 'toolchain: %s\n' "$$($(TARGET_CC) --version 2>/dev/null | head -n1)"; \
	  printf 'config_sha256: %s\n' "$$(sha256sum '$(LEONOS_CONFIG_FILE)' | cut -d' ' -f1)"; \
	  printf 'uapi_sha256: %s\n' "$$(sha256sum '$(HEADER_EXPORT_MANIFEST)' | cut -d' ' -f1)"; \
	  printf 'handoff_version: %s\n' "$$(sed -n 's/^[[:space:]]*#define[[:space:]]\{1,\}LEONOS_BOOT_HANDOFF_VERSION[[:space:]]\{1,\}\([0-9][0-9]*\).*/\1/p' '$(LEONOS_SRC)/include/leonos/boot_handoff.h' | head -n1)"; \
	  printf 'artifacts:\n'; \
	  (cd "$$dest" && sha256sum $(notdir $(INSTALL_PRODUCTS)) | sed 's/^/  /'); \
	} > "$$dest/manifest.txt.tmp"; \
	mv "$$dest/manifest.txt.tmp" "$$dest/manifest.txt"

# --- tests -----------------------------------------------------------------
# The two Python regression tools are boundary tests, not production chain.
# The C host tests cover the shared host primitives and the lock-file reader.
LEONOS_TEST_PYTHON ?= python3
LEONOS_HOST_TEST_BINS := $(O_HOST)/tests/test_common $(O_HOST)/tests/test_json
LEONOS_HOST_TEST_SANITISED := $(O_HOST)/tests-sanitised/test_common \
	$(O_HOST)/tests-sanitised/test_json

$(O_HOST)/obj/tests/host/%.c.o: $(LEONOS_SRC)/tests/host/%.c $(O_META)/host-cc.sig
	$(Q)mkdir -p $(dir $@)
	$(call LEONOS_LOG,HOSTCC,$<)
	$(Q)$(HOSTCC) $(LEONOS_STRICT_WARNINGS) -I$(LEONOS_SRC)/tests/host \
	    $(LEONOS_HOST_INCLUDES) $(HOST_CFLAGS) -MMD -MF $@.d -c $< -o $@

$(O_HOST)/obj/tests-sanitised/host/%.c.o: $(LEONOS_SRC)/tests/host/%.c $(O_META)/host-cc-sanitised.sig
	$(Q)mkdir -p $(dir $@)
	$(call LEONOS_LOG,HOSTCC,$<)
	$(Q)$(HOSTCC) $(LEONOS_STRICT_WARNINGS) $(LEONOS_SANITISE) \
	    -I$(LEONOS_SRC)/tests/host $(LEONOS_HOST_INCLUDES) -g -O1 \
	    -MMD -MF $@.d -c $< -o $@

$(O_HOST)/obj/tests-sanitised/tools/host/%.c.o: $(LEONOS_SRC)/tools/host/%.c \
	$(O_META)/host-cc-sanitised.sig
	$(Q)mkdir -p $(dir $@)
	$(call LEONOS_LOG,HOSTCC,$<)
	$(Q)$(HOSTCC) $(LEONOS_STRICT_WARNINGS) $(LEONOS_SANITISE) -g -O1 \
	    $(LEONOS_HOST_INCLUDES) -MMD -MF $@.d -c $< -o $@

$(O_HOST)/tests/test_common: $(O_HOST)/obj/tests/host/test_common.c.o $(LEONOS_HOST_COMMON_OBJS)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(HOSTCC) $(HOST_CFLAGS) $(HOST_LDFLAGS) $^ -o $@

$(O_HOST)/tests/test_json: $(O_HOST)/obj/tests/host/test_json.c.o $(LEONOS_JSON_OBJ) \
	$(LEONOS_HOST_COMMON_OBJS)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(HOSTCC) $(HOST_CFLAGS) $(HOST_LDFLAGS) $^ -o $@

LEONOS_HOST_SANITISED_OBJS := $(patsubst $(O_HOST)/obj/tools/host/%.c.o, \
	$(O_HOST)/obj/tests-sanitised/tools/host/%.c.o,$(LEONOS_HOST_COMMON_OBJS))

$(O_HOST)/tests-sanitised/test_common: $(O_HOST)/obj/tests-sanitised/host/test_common.c.o \
	$(LEONOS_HOST_SANITISED_OBJS)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(HOSTCC) $(LEONOS_SANITISE) -g -O1 $^ -o $@

$(O_HOST)/tests-sanitised/test_json: $(O_HOST)/obj/tests-sanitised/host/test_json.c.o \
	$(O_HOST)/obj/tests-sanitised/tools/host/manifest/json.c.o $(LEONOS_HOST_SANITISED_OBJS)
	$(Q)mkdir -p $(dir $@)
	$(Q)$(HOSTCC) $(LEONOS_SANITISE) -g -O1 $^ -o $@

LEONOS_SIG_host-cc-sanitised := argv=$(HOSTCC) $(LEONOS_STRICT_WARNINGS) $(LEONOS_SANITISE) -g -O1|path=$(leonos_host_tool_path)|identity=$(leonos_host_tool_identity)
$(if $(LEONOS_PASSIVE),,$(eval $(call LEONOS_SIGNATURE_RULE,host-cc-sanitised)))

-include $(shell find $(O_HOST)/obj/tests $(O_HOST)/obj/tests-sanitised -name '*.o.d' 2>/dev/null)

test-tools: $(LEONOS_HOST_TEST_BINS) $(LEONOS_HOST_TEST_SANITISED)
	@set -eu; for test_binary in $(LEONOS_HOST_TEST_BINS); do \
	    $(call LEONOS_LOG_SHELL,RUN,$$test_binary); \
	    $$test_binary; \
	done
	@set -eu; for test_binary in $(LEONOS_HOST_TEST_SANITISED); do \
	    $(call LEONOS_LOG_SHELL,RUN,$$test_binary); \
	    ASAN_OPTIONS=detect_leaks=1 \
	    UBSAN_OPTIONS=print_stacktrace=1:halt_on_error=1 $$test_binary; \
	done

# UAPI wire-layout freeze against tools/tests/abi_layout_golden.json.
test-abi:
	$(call LEONOS_LOG,RUN,tools/test_abi_layout.py)
	$(Q)$(LEONOS_TEST_PYTHON) $(LEONOS_SRC)/tools/test_abi_layout.py

# headers_install boundary: exact whitelist, self-contained C/C++, no residue.
test-header-export:
	$(call LEONOS_LOG,RUN,tools/test_header_export.py)
	$(Q)$(LEONOS_TEST_PYTHON) $(LEONOS_SRC)/tools/test_header_export.py

test: test-tools test-abi test-header-export

# --- cleaning ---------------------------------------------------------------
# scripts/clean.sh enforces the ownership-marker safety rules: it never removes
# a tree it did not create, and the shared download cache survives every mode.
clean:
	@O='$(O)' SRC='$(LEONOS_SRC)' KEEP_CONFIG=1 sh $(LEONOS_SRC)/scripts/clean.sh

distclean:
	@O='$(O)' SRC='$(LEONOS_SRC)' KEEP_CONFIG=0 sh $(LEONOS_SRC)/scripts/clean.sh
