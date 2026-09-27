/*
 * ReliefOS kernel lifecycle interface: declares top-level kernel initialization.
 * Defines the entry point used after architecture and boot setup complete.
 */
#ifndef RELIEFNT_KERNEL_H
#define RELIEFNT_KERNEL_H

#include <reliefos/boot_handoff.h>

/**
 * @brief Top-level kernel entry point, called once after boot with the loader's handoff data.
 */
void kernel_entry(const struct reliefos_boot_handoff *handoff);
void kernel_idle_loop(void) __attribute__((noreturn));

#endif
