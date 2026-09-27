/*
 * UAPI dual-path compatibility freeze (ReliefOS / ReliefNT rename, plan task 2).
 *
 * The new <reliefos/...> and <reliefnt/...> headers are the single declaration
 * source; the old <leonos/...> and <ntclks/...> headers only forward and alias.
 * Both include spellings must be includable in one translation unit and agree
 * on every published layout and constant value: this test compiles the two
 * paths side by side and compares sizeof / offsetof plus the load-bearing
 * syscall and ioctl constants. No second layout definition may exist behind
 * the old names (plan task 5 pattern: macro alias to the same struct).
 */

#include <stddef.h>

#include <reliefos/system_abi.h>
#include <leonos/system_abi.h>
#include <reliefos/syscall_abi.h>
#include <leonos/syscall_abi.h>
#include <reliefos/gpu_abi.h>
#include <leonos/gpu_abi.h>
#include <reliefos/driver_abi.h>
#include <leonos/driver_abi.h>
#include <reliefos/auth_abi.h>
#include <leonos/auth_abi.h>
#include <reliefnt/version.h>
#include <ntclks/version.h>

/* Including only the old auth_abi path historically also exposed auth_user.
 * Preserve that transitive compatibility for existing <leonos/auth.h>
 * consumers while keeping the ReliefOS declarations canonical. */
#ifndef LEONOS_AUTH_PASSWORD_LEN
#error "leonos/auth_abi.h must preserve auth_user password aliases"
#endif
#ifndef LEONOS_AUTH_PASSWORD_MIN_CHARS
#error "leonos/auth_abi.h must preserve auth_user minimum length alias"
#endif
#ifndef LEONOS_AUTH_PASSWORD_MAX_CHARS
#error "leonos/auth_abi.h must preserve auth_user maximum length alias"
#endif
#ifndef LEONOS_AUTH_USERNAME_LEN
#error "leonos/auth_abi.h must preserve auth_user username length alias"
#endif
#ifndef LEONOS_AUTH_HOME_LEN
#error "leonos/auth_abi.h must preserve auth_user home length alias"
#endif

/* struct leonos_* names must alias the same struct reliefos_* definition, so
 * equal sizes/offsets catch any attempt to publish a diverging second layout. */
_Static_assert(sizeof(struct reliefos_system_info) ==
               sizeof(struct leonos_system_info), "system info ABI changed");
_Static_assert(offsetof(struct reliefos_system_info, kernel_name) ==
               offsetof(struct leonos_system_info, kernel_name),
               "system info kernel_name offset changed");
_Static_assert(offsetof(struct reliefos_system_info, kernel_version) ==
               offsetof(struct leonos_system_info, kernel_version),
               "system info kernel_version offset changed");
_Static_assert(offsetof(struct reliefos_system_info, reserved_subsystem_name) ==
               offsetof(struct leonos_system_info, reserved_subsystem_name),
               "system info reserved_subsystem_name offset changed");
_Static_assert(offsetof(struct reliefos_system_info, build_time) ==
               offsetof(struct leonos_system_info, build_time),
               "system info build_time offset changed");
_Static_assert(offsetof(struct reliefos_system_info, copyright) ==
               offsetof(struct leonos_system_info, copyright),
               "system info copyright offset changed");
_Static_assert(offsetof(struct reliefos_system_info, version_major) ==
               offsetof(struct leonos_system_info, version_major),
               "system info version_major offset changed");
_Static_assert(offsetof(struct reliefos_system_info, copyright_year) ==
               offsetof(struct leonos_system_info, copyright_year),
               "system info copyright_year offset changed");
_Static_assert(offsetof(struct reliefos_system_info, architecture) ==
               offsetof(struct leonos_system_info, architecture),
               "system info architecture offset changed");
_Static_assert(sizeof(struct reliefos_perf_info) ==
               sizeof(struct leonos_perf_info), "perf info ABI changed");
_Static_assert(offsetof(struct reliefos_perf_info, cpus) ==
               offsetof(struct leonos_perf_info, cpus),
               "perf info cpus offset changed");
_Static_assert(sizeof(struct reliefos_task_affinity) ==
               sizeof(struct leonos_task_affinity), "task affinity ABI changed");
_Static_assert(sizeof(struct reliefos_time_info) ==
               sizeof(struct leonos_time_info), "time info ABI changed");
_Static_assert(sizeof(struct reliefos_time_sync) ==
               sizeof(struct leonos_time_sync), "time sync ABI changed");
_Static_assert(sizeof(struct reliefos_machine_identity) ==
               sizeof(struct leonos_machine_identity),
               "machine identity ABI changed");
_Static_assert(sizeof(struct reliefos_user_info) ==
               sizeof(struct leonos_user_info), "user info ABI changed");
_Static_assert(offsetof(struct reliefos_user_info, username) ==
               offsetof(struct leonos_user_info, username),
               "user info username offset changed");
_Static_assert(offsetof(struct reliefos_user_info, home) ==
               offsetof(struct leonos_user_info, home),
               "user info home offset changed");
_Static_assert(LEONOS_AUTH_PASSWORD_LEN == RELIEFOS_AUTH_PASSWORD_LEN,
               "auth password length alias changed");
_Static_assert(LEONOS_AUTH_PASSWORD_MIN_CHARS ==
               RELIEFOS_AUTH_PASSWORD_MIN_CHARS,
               "auth password minimum alias changed");
_Static_assert(LEONOS_AUTH_PASSWORD_MAX_CHARS ==
               RELIEFOS_AUTH_PASSWORD_MAX_CHARS,
               "auth password maximum alias changed");

/* Syscall and ioctl constants keep their published values across the rename. */
_Static_assert(LEONOS_SYS_NICE == RELIEFOS_SYS_NICE, "SYS_NICE value changed");
_Static_assert(LEONOS_IOCTL_GPU_INFO == RELIEFOS_IOCTL_GPU_INFO,
               "IOCTL_GPU_INFO value changed");
_Static_assert(LEONOS_IOCTL_GPU_CREATE == RELIEFOS_IOCTL_GPU_CREATE,
               "IOCTL_GPU_CREATE value changed");
_Static_assert(LEONOS_IOCTL_GPU_DESTROY == RELIEFOS_IOCTL_GPU_DESTROY,
               "IOCTL_GPU_DESTROY value changed");
_Static_assert(LEONOS_IOCTL_GPU_RENDER == RELIEFOS_IOCTL_GPU_RENDER,
               "IOCTL_GPU_RENDER value changed");
_Static_assert(LEONOS_IOCTL_GPU_DIAGNOSTICS == RELIEFOS_IOCTL_GPU_DIAGNOSTICS,
               "IOCTL_GPU_DIAGNOSTICS value changed");
_Static_assert(LEONOS_DRIVER_CONTROL_IOCTL == RELIEFOS_DRIVER_CONTROL_IOCTL,
               "DRIVER_CONTROL_IOCTL value changed");
_Static_assert(LEONOS_TASK_AFFINITY_GET == RELIEFOS_TASK_AFFINITY_GET,
               "TASK_AFFINITY_GET value changed");
_Static_assert(LEONOS_TASK_AFFINITY_SET == RELIEFOS_TASK_AFFINITY_SET,
               "TASK_AFFINITY_SET value changed");
_Static_assert(LEONOS_SYSTEM_NAME_LEN == RELIEFOS_SYSTEM_NAME_LEN,
               "SYSTEM_NAME_LEN value changed");
_Static_assert(LEONOS_PERF_MAX_CPUS == RELIEFOS_PERF_MAX_CPUS,
               "PERF_MAX_CPUS value changed");

/* Both version.h include paths must resolve to one declaration surface. */
_Static_assert(sizeof(&reliefnt_system_info) == sizeof(&ntclks_system_info),
               "system info accessor declaration changed");

int main(void)
{
    return 0;
}
