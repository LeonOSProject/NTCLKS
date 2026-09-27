#ifndef RELIEFOS_UAPI_KERNEL_DEBUG_ABI_H
#define RELIEFOS_UAPI_KERNEL_DEBUG_ABI_H
/*
 * Kernel-debug control wire ABI between ReliefNT and userland. Userland wrappers
 * live in <reliefos/kernel_debug.h>.
 * UAPI only: nothing here may include a non-UAPI header.
 */

#include <stdint.h>

#define RELIEFOS_KERNEL_DEBUG_VERSION 1U

#define RELIEFOS_KERNEL_DEBUG_CONTROL_GET_STATE 1U
#define RELIEFOS_KERNEL_DEBUG_CONTROL_SET_ENABLED 2U
#define RELIEFOS_KERNEL_DEBUG_CONTROL_ARM_NEXT_BOOT 3U
#define RELIEFOS_KERNEL_DEBUG_CONTROL_CLEAR 4U

#define RELIEFOS_KERNEL_DEBUG_STATE_ENABLED 0x00000001U
#define RELIEFOS_KERNEL_DEBUG_STATE_NEXT_BOOT 0x00000002U

struct reliefos_kernel_debug_control {
    uint32_t version;
    uint32_t command;
    uint32_t flags;
    uint32_t result_flags;
};

#endif /* RELIEFOS_UAPI_KERNEL_DEBUG_ABI_H */
