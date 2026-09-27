#ifndef RELIEFOS_UAPI_NET_CONTROL_H
#define RELIEFOS_UAPI_NET_CONTROL_H
#include <reliefos/net_abi.h>
#include <linux/ioctl.h>

/* ReliefOS management extension on an AF_INET socket. Application data uses
 * standard Linux socket syscalls; this does not occupy a Linux syscall ID. */
#define RELIEFOS_NET_CONTROL_VERSION 1u
#define RELIEFOS_NET_CONTROL_CONFIG 1u
#define RELIEFOS_NET_CONTROL_DNS_POLICY 2u
#define RELIEFOS_NET_CONTROL_DHCP 3u
#define RELIEFOS_NET_CONTROL_PING 4u
#define RELIEFOS_NET_CONTROL_CONNECTIONS 5u
struct reliefos_net_control {
    uint32_t version, operation;
    int32_t result;
    uint32_t reserved;
    union {
        struct reliefos_net_config config;
        struct reliefos_net_dns_policy dns_policy;
        struct reliefos_net_dhcp dhcp;
        struct reliefos_net_ping ping;
        struct {
            uint32_t count, reserved;
            struct reliefos_net_connection_info entries[RELIEFOS_NET_SOCKET_MAX];
        } connections;
    } data;
};
#define RELIEFOS_NET_CONTROL_IOCTL _IOWR('L', 0x70, struct reliefos_net_control)
#endif
