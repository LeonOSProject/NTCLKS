/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/rootfs.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_ROOTFS_H
#define LEONOS_ROOTFS_H
#include <reliefos/rootfs.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_DEFAULT_PATH RELIEFOS_DEFAULT_PATH
#define LEONOS_ROOTFS_DIRECTORIES RELIEFOS_ROOTFS_DIRECTORIES
#define LEONOS_ROOTFS_SYMLINKS RELIEFOS_ROOTFS_SYMLINKS
#define leonos_layout reliefos_layout

#endif /* LEONOS_ROOTFS_H */
