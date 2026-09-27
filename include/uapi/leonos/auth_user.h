/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/auth_user.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_UAPI_AUTH_USER_H
#define LEONOS_UAPI_AUTH_USER_H
#include <reliefos/auth_user.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_AUTH_HOME_LEN RELIEFOS_AUTH_HOME_LEN
#define LEONOS_AUTH_PASSWORD_LEN RELIEFOS_AUTH_PASSWORD_LEN
#define LEONOS_AUTH_PASSWORD_MAX_CHARS RELIEFOS_AUTH_PASSWORD_MAX_CHARS
#define LEONOS_AUTH_PASSWORD_MIN_CHARS RELIEFOS_AUTH_PASSWORD_MIN_CHARS
#define LEONOS_AUTH_USERNAME_LEN RELIEFOS_AUTH_USERNAME_LEN
#define leonos_user_info reliefos_user_info

#endif /* LEONOS_UAPI_AUTH_USER_H */
