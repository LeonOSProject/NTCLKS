/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/elf_abi.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_ELF_ABI_H
#define LEONOS_ELF_ABI_H
#include <reliefos/elf_abi.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_ELF_ABI_MAJOR RELIEFOS_ELF_ABI_MAJOR
#define LEONOS_ELF_ABI_MINOR RELIEFOS_ELF_ABI_MINOR
#define LEONOS_ELF_INTERP_PATH RELIEFOS_ELF_INTERP_PATH
#define LEONOS_ELF_NOTE_NAME RELIEFOS_ELF_NOTE_NAME
#define LEONOS_ELF_NOTE_TYPE RELIEFOS_ELF_NOTE_TYPE
#define LEONOS_ELF_RUNTIME_PATH RELIEFOS_ELF_RUNTIME_PATH
#define LEONOS_ELF_RUNTIME_SONAME RELIEFOS_ELF_RUNTIME_SONAME
#define LEONOS_GLIBC_INTERP_PATH RELIEFOS_GLIBC_INTERP_PATH
#define LEONOS_MUSL_INTERP_PATH RELIEFOS_MUSL_INTERP_PATH
#define LEONOS_PATH_GLIBC_INTERP RELIEFOS_PATH_GLIBC_INTERP
#define LEONOS_PATH_LIBLEONOS_COMPAT RELIEFOS_PATH_LIBRELIEFOS_COMPAT
#define LEONOS_PATH_MUSL_INTERP RELIEFOS_PATH_MUSL_INTERP
#define LEONOS_PATH_OLD_NATIVE_INTERP RELIEFOS_PATH_OLD_NATIVE_INTERP
#define leonos_dynamic_launch reliefos_dynamic_launch
#define leonos_elf_abi_note reliefos_elf_abi_note

#endif /* LEONOS_ELF_ABI_H */
