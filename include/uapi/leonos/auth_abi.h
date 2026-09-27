/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/auth_abi.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_UAPI_AUTH_ABI_H
#define LEONOS_UAPI_AUTH_ABI_H
#include <reliefos/auth_abi.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_AUTH_ROLE_ADMIN RELIEFOS_AUTH_ROLE_ADMIN
#define LEONOS_AUTH_ROLE_NONE RELIEFOS_AUTH_ROLE_NONE
#define LEONOS_AUTH_ROLE_USER RELIEFOS_AUTH_ROLE_USER
#define LEONOS_AUTH_UPDATE_FLAGS RELIEFOS_AUTH_UPDATE_FLAGS
#define LEONOS_AUTH_UPDATE_ROLE RELIEFOS_AUTH_UPDATE_ROLE
#define LEONOS_AUTH_USER_DISABLED RELIEFOS_AUTH_USER_DISABLED
#define leonos_auth_status reliefos_auth_status

#endif /* LEONOS_UAPI_AUTH_ABI_H */
