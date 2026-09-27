#ifndef RELIEFOS_UAPI_LINUX_UIO_H
#define RELIEFOS_UAPI_LINUX_UIO_H

#include <linux/types.h>

#define UIO_MAXIOV 1024

struct iovec {
    void *iov_base;
    uint64_t iov_len;
};

#endif
