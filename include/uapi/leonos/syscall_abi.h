/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/syscall_abi.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_UAPI_SYSCALL_ABI_H
#define LEONOS_UAPI_SYSCALL_ABI_H
#include <reliefos/syscall_abi.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_SYS_NICE RELIEFOS_SYS_NICE

#endif /* LEONOS_UAPI_SYSCALL_ABI_H */
