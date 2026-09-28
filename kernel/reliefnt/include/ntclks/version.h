/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical private header: <reliefnt/version.h>.
 * The old private prefix is phasing out; this header only forwards. */
#ifndef NTCLKS_VERSION_H
#define NTCLKS_VERSION_H
#include <reliefnt/version.h>

/* Transitional accessor name; the declaration is reliefnt's. */
#define ntclks_system_info reliefnt_system_info

#endif /* NTCLKS_VERSION_H */
