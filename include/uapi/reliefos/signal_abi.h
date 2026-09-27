#ifndef RELIEFOS_UAPI_SIGNAL_ABI_H
#define RELIEFOS_UAPI_SIGNAL_ABI_H
/*
 * Signal disposition wire ABI shared by ReliefNT and userland. UAPI only:
 * nothing here may include a non-UAPI header.
 */
#include <stdint.h>
#include <linux/signal.h>

/* Minimal process-disposition ABI used by the shared POSIX signal wrappers. */
#define RELIEFOS_SIGNAL_ACTION_GET 1U
#define RELIEFOS_SIGNAL_ACTION_SET 2U
#define RELIEFOS_SIGNAL_DISPOSITION_DEFAULT 0U
#define RELIEFOS_SIGNAL_DISPOSITION_IGNORE 1U

/* Historical source alias. Native frames and records
 * have one owner in UAPI; the former magic/version frame is no longer used. */
#define reliefos_linux_sigaction linux_sigaction

#endif /* RELIEFOS_UAPI_SIGNAL_ABI_H */
