#ifndef RELIEFOS_UAPI_SYSTEM_ABI_H
#define RELIEFOS_UAPI_SYSTEM_ABI_H
/*
 * System wire ABI between ReliefNT and userland. Userland wrappers live in
 * <reliefos/system.h>. UAPI only: nothing here may include a non-UAPI header.
 */

#include <stdint.h>
#include <reliefos/net_abi.h>

#define RELIEFOS_TASK_AFFINITY_GET 0U
#define RELIEFOS_TASK_AFFINITY_SET 1U

#define RELIEFOS_SYSTEM_NAME_LEN 32U
#define RELIEFOS_SYSTEM_VERSION_LEN 32U
#define RELIEFOS_SYSTEM_TIME_LEN 32U
#define RELIEFOS_SYSTEM_COPYRIGHT_LEN 96U
#define RELIEFOS_SYSTEM_ARCH_LEN 16U
#define RELIEFOS_MACHINE_IDENTITY_VERSION 1U
#define RELIEFOS_MACHINE_IDENTITY_SOURCE_LEN 32U
#define RELIEFOS_MACHINE_IDENTITY_UUID_LEN 37U
#define RELIEFOS_MACHINE_IDENTITY_VENDOR_LEN 48U

#define RELIEFOS_MACHINE_IDENTITY_FLAG_PLATFORM_UUID 0x00000001U
#define RELIEFOS_MACHINE_IDENTITY_FLAG_BOOT_DISK_GUID 0x00000002U
#define RELIEFOS_MACHINE_IDENTITY_FLAG_BOOT_PARTITION_GUID 0x00000004U
#define RELIEFOS_PERF_MAX_CPUS 64U

struct reliefos_system_info {
    char kernel_name[RELIEFOS_SYSTEM_NAME_LEN];
    char kernel_version[RELIEFOS_SYSTEM_VERSION_LEN];
    /* Formerly the middle-layer module name.  The slot is kept so every other
     * field stays at its published offset, and the kernel always reports it
     * empty: there is no second boot module to name. */
    char reserved_subsystem_name[RELIEFOS_SYSTEM_NAME_LEN];
    char build_time[RELIEFOS_SYSTEM_TIME_LEN];
    char copyright[RELIEFOS_SYSTEM_COPYRIGHT_LEN];
    uint32_t version_major;
    uint32_t version_minor;
    uint32_t version_patch;
    uint32_t reserved_version; /* Reserved; keep system-info ABI offsets stable. */
    uint32_t copyright_year;
    char architecture[RELIEFOS_SYSTEM_ARCH_LEN];
};

struct reliefos_perf_cpu_info {
    uint64_t busy_ticks;
    uint64_t idle_ticks;
    uint32_t online;
    uint32_t apic_id;
    uint32_t current_pid;
    uint32_t runqueue_length;
};

struct reliefos_perf_info {
    uint64_t uptime_ms;
    uint64_t total_memory_kib;
    uint64_t free_memory_kib;
    uint64_t busy_ticks;
    uint64_t idle_ticks;
    uint32_t task_count;
    uint32_t running_tasks;
    uint32_t ready_tasks;
    uint32_t sleeping_tasks;
    uint32_t cpu_count;
    uint32_t reserved;
    uint64_t sample_tick;
    uint32_t online_cpu_count;
    uint32_t reserved2;
    struct reliefos_perf_cpu_info cpus[RELIEFOS_PERF_MAX_CPUS];
};

struct reliefos_task_affinity {
    uint32_t pid;
    uint32_t operation;
    uint64_t mask;
    uint64_t allowed_mask;
};

/* The broken-down calendar is UTC, matching unix_seconds. */
struct reliefos_time_info {
    uint64_t unix_seconds;
    uint64_t uptime_ms;
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
    uint32_t valid;
    uint32_t reserved;
};

struct reliefos_time_sync {
    uint32_t timeout_ms;
    uint32_t status;
    uint32_t server_ip;
    uint32_t valid;
    uint64_t unix_seconds;
    char server[RELIEFOS_NET_HOSTNAME_LEN];
};

struct reliefos_machine_identity {
    uint32_t version;
    uint32_t flags;
    char source[RELIEFOS_MACHINE_IDENTITY_SOURCE_LEN];
    char platform_uuid[RELIEFOS_MACHINE_IDENTITY_UUID_LEN];
    char boot_disk_guid[RELIEFOS_MACHINE_IDENTITY_UUID_LEN];
    char boot_partition_guid[RELIEFOS_MACHINE_IDENTITY_UUID_LEN];
    char firmware_vendor[RELIEFOS_MACHINE_IDENTITY_VENDOR_LEN];
    uint32_t firmware_revision;
    uint32_t reserved;
};

#endif /* RELIEFOS_UAPI_SYSTEM_ABI_H */
