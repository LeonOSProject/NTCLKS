/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/net_control.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_UAPI_NET_CONTROL_H
#define LEONOS_UAPI_NET_CONTROL_H
#include <reliefos/net_control.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_NET_CONTROL_CONFIG RELIEFOS_NET_CONTROL_CONFIG
#define LEONOS_NET_CONTROL_CONNECTIONS RELIEFOS_NET_CONTROL_CONNECTIONS
#define LEONOS_NET_CONTROL_DHCP RELIEFOS_NET_CONTROL_DHCP
#define LEONOS_NET_CONTROL_DNS_POLICY RELIEFOS_NET_CONTROL_DNS_POLICY
#define LEONOS_NET_CONTROL_IOCTL RELIEFOS_NET_CONTROL_IOCTL
#define LEONOS_NET_CONTROL_PING RELIEFOS_NET_CONTROL_PING
#define LEONOS_NET_CONTROL_VERSION RELIEFOS_NET_CONTROL_VERSION
#define LEONOS_NET_SOCKET_MAX RELIEFOS_NET_SOCKET_MAX
#define leonos_net_config reliefos_net_config
#define leonos_net_connection_info reliefos_net_connection_info
#define leonos_net_control reliefos_net_control
#define leonos_net_dhcp reliefos_net_dhcp
#define leonos_net_dns_policy reliefos_net_dns_policy
#define leonos_net_ping reliefos_net_ping

#endif /* LEONOS_UAPI_NET_CONTROL_H */
