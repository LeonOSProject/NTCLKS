/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/psf_font.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_PSF_FONT_H
#define LEONOS_PSF_FONT_H
#include <reliefos/psf_font.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_FONT_H RELIEFOS_FONT_H
#define LEONOS_FONT_W RELIEFOS_FONT_W
#define LEONOS_PATH_SYSTEM_FONT RELIEFOS_PATH_SYSTEM_FONT
#define LEONOS_SYSTEM_FONT_PATH RELIEFOS_SYSTEM_FONT_PATH
#define leonos_lat15_vga16_psf reliefos_lat15_vga16_psf
#define leonos_lat15_vga16_psf_len reliefos_lat15_vga16_psf_len
#define leonos_psf_glyph reliefos_psf_glyph
#define leonos_psf_view reliefos_psf_view
#define leonos_psf_view_from_memory reliefos_psf_view_from_memory
#define leonos_psf_view_glyph reliefos_psf_view_glyph

#endif /* LEONOS_PSF_FONT_H */
