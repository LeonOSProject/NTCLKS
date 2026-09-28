#ifndef RELIEFOS_UAPI_SYSCALL_ABI_H
#define RELIEFOS_UAPI_SYSCALL_ABI_H

/* Legacy ReliefOS nice extension; native Linux x86-64 syscall 34 is pause.
 * New libc consumers implement nice through getpriority/setpriority. */
#define RELIEFOS_SYS_NICE 0x10000u

#endif
