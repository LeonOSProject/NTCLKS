#ifndef RELIEFOS_UAPI_AUTH_USER_H
#define RELIEFOS_UAPI_AUTH_USER_H
#include <stdint.h>

#define RELIEFOS_AUTH_USERNAME_LEN 32U
#define RELIEFOS_AUTH_PASSWORD_MIN_CHARS 1U
#define RELIEFOS_AUTH_PASSWORD_MAX_CHARS 32U
/* At most 32 UTF-8 scalar values plus the terminator. */
#define RELIEFOS_AUTH_PASSWORD_LEN (RELIEFOS_AUTH_PASSWORD_MAX_CHARS * 4U + 1U)
#define RELIEFOS_AUTH_HOME_LEN 96U

struct reliefos_user_info {
    uint32_t uid;
    uint32_t role;
    uint32_t flags;
    uint32_t reserved;
    char username[RELIEFOS_AUTH_USERNAME_LEN];
    char home[RELIEFOS_AUTH_HOME_LEN];
};
#endif
