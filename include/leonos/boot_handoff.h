/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/boot_handoff.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_BOOT_HANDOFF_H
#define LEONOS_BOOT_HANDOFF_H
#include <reliefos/boot_handoff.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_BOOT_HANDOFF_MAGIC RELIEFOS_BOOT_HANDOFF_MAGIC
#define LEONOS_BOOT_HANDOFF_VERSION RELIEFOS_BOOT_HANDOFF_VERSION
#define leonos_boot_handoff reliefos_boot_handoff
#define leonos_boot_log_state reliefos_boot_log_state
#define leonos_boot_module_info reliefos_boot_module_info

#endif /* LEONOS_BOOT_HANDOFF_H */
