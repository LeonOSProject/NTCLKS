/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/signal_abi.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_UAPI_SIGNAL_ABI_H
#define LEONOS_UAPI_SIGNAL_ABI_H
#include <reliefos/signal_abi.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_SIGNAL_ACTION_GET RELIEFOS_SIGNAL_ACTION_GET
#define LEONOS_SIGNAL_ACTION_SET RELIEFOS_SIGNAL_ACTION_SET
#define LEONOS_SIGNAL_DISPOSITION_DEFAULT RELIEFOS_SIGNAL_DISPOSITION_DEFAULT
#define LEONOS_SIGNAL_DISPOSITION_IGNORE RELIEFOS_SIGNAL_DISPOSITION_IGNORE
#define leonos_linux_sigaction reliefos_linux_sigaction

#endif /* LEONOS_UAPI_SIGNAL_ABI_H */
