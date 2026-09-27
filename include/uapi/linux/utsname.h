#ifndef RELIEFOS_UAPI_LINUX_UTSNAME_H
#define RELIEFOS_UAPI_LINUX_UTSNAME_H

#define RELIEFOS_UTSNAME_LEN 65u

struct utsname {
    char sysname[RELIEFOS_UTSNAME_LEN];
    char nodename[RELIEFOS_UTSNAME_LEN];
    char release[RELIEFOS_UTSNAME_LEN];
    char version[RELIEFOS_UTSNAME_LEN];
    char machine[RELIEFOS_UTSNAME_LEN];
    char domainname[RELIEFOS_UTSNAME_LEN];
};

#endif
