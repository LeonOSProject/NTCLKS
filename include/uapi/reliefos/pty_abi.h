#ifndef RELIEFOS_UAPI_PTY_ABI_H
#define RELIEFOS_UAPI_PTY_ABI_H
/*
 * PTY wire ABI: Linux termios alias layer and winsize shared by ReliefNT and
 * userland. Userland wrappers live in <reliefos/pty.h>.
 * UAPI only: nothing here may include a non-UAPI header.
 */


#include <stdint.h>
#include <linux/termios.h>

#define RELIEFOS_PTY_PATH_LEN 160U
#define RELIEFOS_PTY_NCCS LINUX_NCCS
#define RELIEFOS_PTY_IFLAG_ICRNL LINUX_ICRNL
#define RELIEFOS_PTY_LFLAG_ECHO LINUX_ECHO
#define RELIEFOS_PTY_LFLAG_ECHONL LINUX_ECHONL
#define RELIEFOS_PTY_LFLAG_ICANON LINUX_ICANON
#define RELIEFOS_PTY_LFLAG_IEXTEN LINUX_IEXTEN
#define RELIEFOS_PTY_LFLAG_ISIG LINUX_ISIG

#define RELIEFOS_PTY_CC_VEOF LINUX_VEOF
#define RELIEFOS_PTY_CC_VEOL LINUX_VEOL
#define RELIEFOS_PTY_CC_VERASE LINUX_VERASE
#define RELIEFOS_PTY_CC_VINTR LINUX_VINTR
#define RELIEFOS_PTY_CC_VKILL LINUX_VKILL
#define RELIEFOS_PTY_CC_VMIN LINUX_VMIN
#define RELIEFOS_PTY_CC_VQUIT LINUX_VQUIT
#define RELIEFOS_PTY_CC_VSTART LINUX_VSTART
#define RELIEFOS_PTY_CC_VSTOP LINUX_VSTOP
#define RELIEFOS_PTY_CC_VSUSP LINUX_VSUSP
#define RELIEFOS_PTY_CC_VTIME LINUX_VTIME

/* Source compatibility only: old Picolibc wire layouts require rebuilding. */
#define reliefos_pty_termios linux_termios2

/* The terminal host owns a session but is not itself attached to it. Keep
 * host-side controls separate from the stdio-based child-process requests. */
struct reliefos_pty_winsize {
    uint16_t ws_row;
    uint16_t ws_col;
};

#endif /* RELIEFOS_UAPI_PTY_ABI_H */
