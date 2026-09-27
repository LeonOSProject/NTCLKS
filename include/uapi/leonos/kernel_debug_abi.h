/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/kernel_debug_abi.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_UAPI_KERNEL_DEBUG_ABI_H
#define LEONOS_UAPI_KERNEL_DEBUG_ABI_H
#include <reliefos/kernel_debug_abi.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_KERNEL_DEBUG_CONTROL_ARM_NEXT_BOOT RELIEFOS_KERNEL_DEBUG_CONTROL_ARM_NEXT_BOOT
#define LEONOS_KERNEL_DEBUG_CONTROL_CLEAR RELIEFOS_KERNEL_DEBUG_CONTROL_CLEAR
#define LEONOS_KERNEL_DEBUG_CONTROL_GET_STATE RELIEFOS_KERNEL_DEBUG_CONTROL_GET_STATE
#define LEONOS_KERNEL_DEBUG_CONTROL_SET_ENABLED RELIEFOS_KERNEL_DEBUG_CONTROL_SET_ENABLED
#define LEONOS_KERNEL_DEBUG_STATE_ENABLED RELIEFOS_KERNEL_DEBUG_STATE_ENABLED
#define LEONOS_KERNEL_DEBUG_STATE_NEXT_BOOT RELIEFOS_KERNEL_DEBUG_STATE_NEXT_BOOT
#define LEONOS_KERNEL_DEBUG_VERSION RELIEFOS_KERNEL_DEBUG_VERSION
#define leonos_kernel_debug_control reliefos_kernel_debug_control

#endif /* LEONOS_UAPI_KERNEL_DEBUG_ABI_H */
