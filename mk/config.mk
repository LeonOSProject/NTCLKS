# Configuration: the Kconfig front end, the derived build inputs, and the
# command-line override guard.
#
# Kconfig is the authority for feature selection and PROFILE for compile policy.
# Nothing here keeps a second copy of the defaults in a C constant or a Make
# variable (plan section 7). The parent product configuration has no place
# here: components.toml and the installed-system (installer) policy header are
# parent-side concerns and the kernel must not read product config.

KCONFIG_SEED := $(LEONOS_SRC)/configs/default.conf
KCONFIG_ROOT := $(LEONOS_SRC)/Kconfig
LEONOS_CONFIG_FILE := $(O_CONFIG)/.config

# The repository-pinned C Kconfig front end. Built into the output tree because
# `make tools` is reserved for this project's own C helpers (plan section 4).
KCONFIG_PREFIX := $(O_HOST)/kconfig-frontends
KCONFIG_CONF := $(KCONFIG_PREFIX)/bin/kconfig-conf
KCONFIG_MCONF := $(KCONFIG_PREFIX)/bin/kconfig-mconf

AUTOCONF_H := $(O_INCLUDE)/generated/autoconf.h
LEONOS_AUTOCONF_MK := $(O_CONFIG)/autoconf.mk

# --- command-line override guard --------------------------------------------
# `make CONFIG_SOMETHING=y` must not be accepted and then ignored. Make has no
# regex, but $(origin) distinguishes a command-line assignment from a builtin or
# file one, and the Kconfig symbol namespace is plain data.
ifneq ($(LEONOS_PASSIVE),1)
leonos_cmdline_config_keys := $(foreach v,$(filter CONFIG_%,$(.VARIABLES)), \
	$(if $(filter command line,$(origin $(v))),$(v),))
leonos_kconfig_symbols := $(if $(leonos_cmdline_config_keys), \
	$(shell sed -n 's/^[[:space:]]*config[[:space:]]\{1,\}\([A-Za-z0-9_]\{1,\}\)$$/\1/p' \
		$(KCONFIG_ROOT) $(LEONOS_SRC)/Kconfig.components 2>/dev/null | sort -u),)
leonos_unknown_config_keys := $(filter-out $(leonos_kconfig_symbols),$(leonos_cmdline_config_keys))
$(if $(leonos_unknown_config_keys),$(error unknown configuration key(s) on the command line: $(leonos_unknown_config_keys)))
endif

# --- generated headers ------------------------------------------------------
# One autoconf header for the kernel build; the parent's second
# installed-system policy header is userland-side and is not generated here.
$(AUTOCONF_H) $(LEONOS_AUTOCONF_MK) &: \
	$(LEONOS_CONFIG_FILE) $(LEONOS_CONFIG_TOOL) | $(O_INCLUDE)/generated
	$(call LEONOS_LOG,GEN,$@)
	$(Q)$(LEONOS_CONFIG_TOOL) --input $(LEONOS_CONFIG_FILE) \
		--out-header $(AUTOCONF_H) --guard LEONOS4_AUTOCONF_H \
		--require-license CONFIG_VMDK_REQUIRE_LICENSE \
		--make-include $(LEONOS_AUTOCONF_MK)

# --- the .config itself -----------------------------------------------------
# Absent configuration is initialised from the committed profile, but an existing
# .config is never silently rewritten: only an explicit defconfig, olddefconfig
# or menuconfig goal changes it.
$(LEONOS_CONFIG_FILE): $(KCONFIG_SEED) $(KCONFIG_ROOT) $(LEONOS_SRC)/Kconfig.components $(KCONFIG_CONF)
	$(call LEONOS_LOG,CONFIG,$@)
	$(Q)sh $(LEONOS_SRC)/tools/build/kconfig-frontends.sh run \
		--conf $(abspath $(KCONFIG_CONF)) --mconf $(abspath $(KCONFIG_MCONF)) \
		--kconfig $(KCONFIG_ROOT) --config $(abspath $@) --seed $(KCONFIG_SEED) \
		--mode ''

# Rebuild the derived files when the Kconfig inputs change even if .config was
# produced by hand.
$(LEONOS_CONFIG_FILE): | $(O_CONFIG)

# --- host build of the pinned front end ------------------------------------
$(KCONFIG_CONF) $(KCONFIG_MCONF) &: $(LEONOS_SRC)/$(LEONOS_HOST_PUFF_SRC) \
	$(LEONOS_SRC)/tools/build/kconfig-frontends.sh
	$(Q)sh $(LEONOS_SRC)/tools/build/kconfig-frontends.sh build \
		--source $(LEONOS_SRC)/third_party/kconfig-frontends \
		--work $(abspath $(O_HOST)/kconfig-frontends-build) \
		--prefix $(abspath $(KCONFIG_PREFIX))

# Make restarts after a generated include is rebuilt. The include is produced by
# leonos-config, which publishes only on content change, so a stable
# configuration cannot loop; the guard below catches the pathological case
# instead of hanging (plan section 7).
ifeq ($(LEONOS_PASSIVE),1)
else
ifneq ($(LEONOS_INSPECT),)
ifneq ($(wildcard $(LEONOS_AUTOCONF_MK)),)
$(eval $(file <$(LEONOS_AUTOCONF_MK)))
endif
else
# Include even when absent: Make must generate this file and restart before
# expanding recipes that consume KCONFIG_* on the first clean build.
-include $(LEONOS_AUTOCONF_MK)
endif
endif
