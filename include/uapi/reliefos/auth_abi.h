#ifndef RELIEFOS_UAPI_AUTH_ABI_H
#define RELIEFOS_UAPI_AUTH_ABI_H
/*
 * Authentication wire ABI between ReliefNT and userland. Userland wrappers live
 * in <reliefos/auth.h>. UAPI only: nothing here may include a non-UAPI header.
 */

#include <stdint.h>
#include <reliefos/auth_user.h>

#define RELIEFOS_AUTH_ROLE_NONE 0U
#define RELIEFOS_AUTH_ROLE_USER 1U
#define RELIEFOS_AUTH_ROLE_ADMIN 2U

#define RELIEFOS_AUTH_USER_DISABLED 0x00000001U

#define RELIEFOS_AUTH_UPDATE_ROLE 0x00000001U
#define RELIEFOS_AUTH_UPDATE_FLAGS 0x00000002U

struct reliefos_auth_status {
    uint32_t user_count;
    uint32_t has_admin;
    uint32_t reserved0;
    uint32_t reserved1;
};

#endif /* RELIEFOS_UAPI_AUTH_ABI_H */
