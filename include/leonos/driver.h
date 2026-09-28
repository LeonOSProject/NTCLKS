/* Transitional compatibility forwarder (ReliefOS / ReliefNT rename).
 * Canonical declarations: <reliefos/driver.h>.
 * No second layout definition exists behind these names. */
#ifndef LEONOS_DRIVER_H
#define LEONOS_DRIVER_H
#include <reliefos/driver.h>

/* Old names are macro aliases to the same declarations. */
#define LEONOS_DRIVER_ABI_VERSION RELIEFOS_DRIVER_ABI_VERSION
#define LEONOS_DRIVER_KIND_AUDIO RELIEFOS_DRIVER_KIND_AUDIO
#define LEONOS_DRIVER_KIND_INPUT RELIEFOS_DRIVER_KIND_INPUT
#define LEONOS_DRIVER_KIND_NETWORK RELIEFOS_DRIVER_KIND_NETWORK
#define LEONOS_DRIVER_KIND_SERIAL RELIEFOS_DRIVER_KIND_SERIAL
#define LEONOS_DRIVER_MAX RELIEFOS_DRIVER_MAX
#define LEONOS_DRIVER_MODULE_MAGIC RELIEFOS_DRIVER_MODULE_MAGIC
#define LEONOS_DRIVER_NAME_LEN RELIEFOS_DRIVER_NAME_LEN
#define leonos_audio_format reliefos_audio_format
#define leonos_audio_state reliefos_audio_state
#define leonos_driver_audio_ops reliefos_driver_audio_ops
#define leonos_driver_control reliefos_driver_control
#define leonos_driver_e1000_info reliefos_driver_e1000_info
#define leonos_driver_e1000_ops reliefos_driver_e1000_ops
#define leonos_driver_info reliefos_driver_info
#define leonos_driver_kernel_api reliefos_driver_kernel_api
#define leonos_driver_list reliefos_driver_list
#define leonos_driver_module reliefos_driver_module
#define leonos_driver_mouse_ops reliefos_driver_mouse_ops
#define leonos_driver_mouse_state reliefos_driver_mouse_state
#define leonos_driver_pci_device reliefos_driver_pci_device
#define leonos_driver_serial_ops reliefos_driver_serial_ops

#endif /* LEONOS_DRIVER_H */
