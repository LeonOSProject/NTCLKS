#ifndef RELIEFOS_UAPI_NET_ABI_H
#define RELIEFOS_UAPI_NET_ABI_H
/*
 * Network wire ABI between ReliefNT and userland. Userland wrappers live in
 * <reliefos/net.h>; the management ioctl envelope is <reliefos/net_control.h>.
 * UAPI only: nothing here may include a non-UAPI header.
 */

#include <stdint.h>


#define RELIEFOS_NET_STATUS_OK 0U
#define RELIEFOS_NET_STATUS_NO_DEVICE 1U
#define RELIEFOS_NET_STATUS_ARP_TIMEOUT 2U
#define RELIEFOS_NET_STATUS_ECHO_TIMEOUT 3U
#define RELIEFOS_NET_STATUS_BAD_ARGUMENT 4U
#define RELIEFOS_NET_STATUS_TX_FAILED 5U
#define RELIEFOS_NET_STATUS_DHCP_TIMEOUT 6U
#define RELIEFOS_NET_STATUS_DHCP_FAILED 7U
#define RELIEFOS_NET_STATUS_DNS_TIMEOUT 8U
#define RELIEFOS_NET_STATUS_DNS_FAILED 9U
#define RELIEFOS_NET_STATUS_DNS_NO_ANSWER 10U
#define RELIEFOS_NET_STATUS_TCP_TIMEOUT 11U
#define RELIEFOS_NET_STATUS_TCP_RESET 12U
#define RELIEFOS_NET_STATUS_TCP_FAILED 13U
#define RELIEFOS_NET_STATUS_HTTP_FAILED 14U
#define RELIEFOS_NET_STATUS_HTTP_TOO_LARGE 15U
#define RELIEFOS_NET_STATUS_SOCKET_LIMIT 16U
#define RELIEFOS_NET_STATUS_SOCKET_BAD_HANDLE 17U
#define RELIEFOS_NET_STATUS_SOCKET_NOT_CONNECTED 18U
#define RELIEFOS_NET_STATUS_SOCKET_CLOSED 19U
#define RELIEFOS_NET_STATUS_PROTOCOL_UNSUPPORTED 20U
#define RELIEFOS_NET_STATUS_TLS_FAILED 21U
#define RELIEFOS_NET_STATUS_NTP_TIMEOUT 22U
#define RELIEFOS_NET_STATUS_NTP_INVALID 23U
#define RELIEFOS_NET_STATUS_NO_ADDRESS 24U

#define RELIEFOS_NET_DEFAULT_TIMEOUT_MS 1000U
#define RELIEFOS_NET_MAX_TIMEOUT_MS 10000U
#define RELIEFOS_NET_DEFAULT_LOCAL_IP 0x0a00020fU
#define RELIEFOS_NET_DEFAULT_GATEWAY_IP 0x0a000202U
#define RELIEFOS_NET_DEFAULT_SUBNET_MASK 0xffffff00U
#define RELIEFOS_NET_CLOUDFLARE_DNS_IP 0x01010101U
#define RELIEFOS_NET_DEFAULT_DNS_IP RELIEFOS_NET_CLOUDFLARE_DNS_IP

#define RELIEFOS_NET_DNS_MODE_CLOUDFLARE 0U
#define RELIEFOS_NET_DNS_MODE_DHCP 1U
#define RELIEFOS_NET_DNS_MODE_CUSTOM 2U
#define RELIEFOS_NET_DNS_MODE_QUERY 0xffffffffU

#define RELIEFOS_NET_CONFIG_SOURCE_NONE 0U
#define RELIEFOS_NET_CONFIG_SOURCE_STATIC 1U
#define RELIEFOS_NET_CONFIG_SOURCE_DHCP 2U

#define RELIEFOS_NET_CONFIG_FLAG_PRESENT 0x00000001U
#define RELIEFOS_NET_CONFIG_FLAG_ACTIVE 0x00000002U
#define RELIEFOS_NET_CONFIG_FLAG_DHCP 0x00000004U

#define RELIEFOS_NET_HOSTNAME_LEN 128U
#define RELIEFOS_NET_DNS_MAX_ADDRESSES 4U
#define RELIEFOS_NET_HTTP_PATH_LEN 256U
#define RELIEFOS_NET_HTTP_RESPONSE_MAX 4096U
#define RELIEFOS_NET_SOCKET_MAX 16U

#define RELIEFOS_NET_AF_INET 2U
#define RELIEFOS_NET_SOCK_STREAM 1U
#define RELIEFOS_NET_IPPROTO_TCP 6U

#define RELIEFOS_NET_TCP_CLOSED 0U
#define RELIEFOS_NET_TCP_SYN_SENT 1U
#define RELIEFOS_NET_TCP_ESTABLISHED 2U
#define RELIEFOS_NET_TCP_TIME_WAIT 3U

struct reliefos_net_config {
    uint32_t flags;
    uint32_t source;
    uint32_t local_ip;
    uint32_t subnet_mask;
    uint32_t gateway_ip;
    uint32_t dns_ip;
    uint32_t dhcp_server_ip;
    uint32_t lease_seconds;
    uint8_t mac[6];
    uint8_t reserved_mac[2];
};

struct reliefos_net_dhcp {
    uint32_t timeout_ms;
    uint32_t status;
    struct reliefos_net_config config;
};

struct reliefos_net_dns_policy {
    uint32_t mode;
    uint32_t custom_dns_ip;
    uint32_t status;
    uint32_t reserved;
    struct reliefos_net_config config;
};

struct reliefos_net_ping {
    uint32_t target_ip;
    uint32_t timeout_ms;
    uint32_t sequence;
    uint32_t status;
    uint32_t rtt_ms;
    uint32_t sent;
    uint32_t received;
    uint32_t reserved;
};

struct reliefos_net_dns {
    char name[RELIEFOS_NET_HOSTNAME_LEN];
    uint32_t timeout_ms;
    uint32_t status;
    uint32_t address_count;
    uint32_t addresses[RELIEFOS_NET_DNS_MAX_ADDRESSES];
};

struct reliefos_net_http_get {
    char host[RELIEFOS_NET_HOSTNAME_LEN];
    char path[RELIEFOS_NET_HTTP_PATH_LEN];
    uint32_t port;
    uint32_t timeout_ms;
    uint32_t status;
    uint32_t remote_ip;
    uint32_t http_status;
    uint32_t response_len;
    char response[RELIEFOS_NET_HTTP_RESPONSE_MAX];
};

struct reliefos_net_socket_open {
    uint32_t domain;
    uint32_t type;
    uint32_t protocol;
    uint32_t timeout_ms;
    uint32_t status;
    int32_t socket;
};

struct reliefos_net_socket_connect {
    int32_t socket;
    char host[RELIEFOS_NET_HOSTNAME_LEN];
    uint32_t port;
    uint32_t timeout_ms;
    uint32_t status;
    uint32_t remote_ip;
    uint32_t local_ip;
    uint32_t local_port;
};

struct reliefos_net_socket_io {
    int32_t socket;
    void *buffer;
    uint32_t length;
    uint32_t timeout_ms;
    uint32_t status;
    uint32_t transferred;
};

struct reliefos_net_socket_close {
    int32_t socket;
    uint32_t status;
};

struct reliefos_net_connection_info {
    int32_t socket;
    uint32_t owner_pid;
    uint32_t state;
    uint32_t status;
    uint32_t local_ip;
    uint32_t remote_ip;
    uint32_t local_port;
    uint32_t remote_port;
    uint32_t age_ms;
    uint32_t tx_bytes;
    uint32_t rx_bytes;
};

struct reliefos_net_connection_list {
    uint32_t capacity;
    uint32_t count;
    struct reliefos_net_connection_info *entries;
};

#endif /* RELIEFOS_UAPI_NET_ABI_H */
