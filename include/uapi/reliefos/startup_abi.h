#ifndef RELIEFOS_UAPI_STARTUP_ABI_H
#define RELIEFOS_UAPI_STARTUP_ABI_H
/*
 * Startup-approval IPC wire ABI between sessiond and userland.
 * Userland wrappers live in <reliefos/startup.h>.
 * UAPI only: nothing here may include a non-UAPI header.
 */

#include <stdint.h>


#define RELIEFOS_STARTUP_MAX_ENTRIES 16U
#define RELIEFOS_STARTUP_MAX_ARGS 7U
#define RELIEFOS_STARTUP_ARG_LEN 64U

#define RELIEFOS_STARTUP_STATUS_PENDING 1U
#define RELIEFOS_STARTUP_STATUS_APPROVED 2U
#define RELIEFOS_STARTUP_STATUS_DENIED 3U
#define RELIEFOS_STARTUP_STATUS_DENIED_REMEMBERED 4U
#define RELIEFOS_STARTUP_STATUS_EXISTS 5U
#define RELIEFOS_STARTUP_STATUS_CANCELLED 6U
#define RELIEFOS_STARTUP_STATUS_FAILED 7U

#define RELIEFOS_STARTUP_DECISION_ALLOW 1U
#define RELIEFOS_STARTUP_DECISION_DENY 2U
#define RELIEFOS_STARTUP_DECISION_DENY_REMEMBERED 3U

/* args excludes argv[0]; the executable path is always argv[0]. */
struct reliefos_startup_command {
    uint32_t argc;
    uint32_t reserved;
    char path[256];
    char args[RELIEFOS_STARTUP_MAX_ARGS][RELIEFOS_STARTUP_ARG_LEN];
};

struct reliefos_startup_request {
    struct reliefos_startup_command command;
    uint32_t request_id;
    uint32_t status;
};

struct reliefos_startup_request_status {
    uint32_t request_id;
    uint32_t status;
};

struct reliefos_startup_dialog_request {
    uint32_t request_id;
    uint32_t uid;
    char requester_path[256];
    struct reliefos_startup_command command;
};

struct reliefos_startup_dialog_resolution {
    uint32_t request_id;
    uint32_t decision;
};

struct reliefos_startup_entry {
    uint32_t id;
    uint32_t enabled;
    struct reliefos_startup_command command;
};

struct reliefos_startup_list {
    uint32_t uid;
    uint32_t capacity;
    uint32_t count;
    uint32_t reserved;
    struct reliefos_startup_entry *entries;
};

struct reliefos_startup_update {
    uint32_t uid;
    uint32_t entry_id;
    uint32_t enabled;
    uint32_t reserved;
};

#endif /* RELIEFOS_UAPI_STARTUP_ABI_H */
