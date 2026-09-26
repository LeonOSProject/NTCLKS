# Generated resource headers consumed by the kernel build.
#
# Only the CJK font pipeline migrates with the kernel: cjk_font.h feeds
# drivers/bootstrap/framebuffer.c and is the one generated asset in the kernel
# link (migration manifest section 3, hazard 5). The unifont dependency
# closure travels with it (configs/dependencies.lock.json, tools/build/fetch.sh,
# tools/host/manifest/leonos-deps, resources/licenses/unifont-LICENSE). The UI
# and GRUB font rules stay with the parent repository.

LEONOS_CJK_FONT_TOOL := $(LEONOS_HOST_BIN)/leonos-cjk-font
$(LEONOS_CJK_FONT_TOOL): $(O_HOST)/obj/tools/host/assets/leonos-cjk-font.c.o | $(LEONOS_HOST_BIN)
	$(Q)$(HOSTCC) $(HOST_CFLAGS) $(HOST_LDFLAGS) $^ -o $@

$(O_INCLUDE)/generated/cjk_font.h: $(LEONOS_CJK_FONT_TOOL) $(LEONOS_DEPS_TOOL) $(LEONOS_SRC)/configs/dependencies.lock.json $(LEONOS_SRC)/mk/resources.mk
	$(Q)sh $(LEONOS_SRC)/tools/build/fetch.sh --deps $(LEONOS_DEPS_TOOL) --lock $(LEONOS_SRC)/configs/dependencies.lock.json --cache $(LEONOS_CACHE) --only unifont --verify-only
	$(Q)mkdir -p $(@D)
	$(Q)set -eu; font=$$($(LEONOS_DEPS_TOOL) --lock $(LEONOS_SRC)/configs/dependencies.lock.json --id unifont --print directory); gzip -dc $(LEONOS_CACHE)/$$font > $@.hex; $(LEONOS_CJK_FONT_TOOL) < $@.hex > $@.tmp; mv $@.tmp $@; rm -f $@.hex

$(O_OBJ)/kernel/drivers/bootstrap/framebuffer.c.o: $(O_INCLUDE)/generated/cjk_font.h
tools: $(LEONOS_CJK_FONT_TOOL)
