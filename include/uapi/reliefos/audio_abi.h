#ifndef RELIEFOS_UAPI_AUDIO_ABI_H
#define RELIEFOS_UAPI_AUDIO_ABI_H
/*
 * Audio wire ABI between ReliefNT and userland. Userland wrappers live in
 * <reliefos/audio.h>.
 * UAPI only: nothing here may include a non-UAPI header.
 */

#include <stdint.h>


#define RELIEFOS_AUDIO_MAX_WRITE (64U * 1024U)
/* Audio drivers poll hardware synchronously; keep each kernel call bounded. */
#define RELIEFOS_AUDIO_IO_SLICE_BYTES 4096U

#define RELIEFOS_AUDIO_STATUS_OK 0U
#define RELIEFOS_AUDIO_STATUS_NO_DEVICE 1U
#define RELIEFOS_AUDIO_STATUS_BAD_FORMAT 2U
#define RELIEFOS_AUDIO_STATUS_PLAYBACK_FAILED 3U
#define RELIEFOS_AUDIO_STATUS_WOULD_BLOCK 4U

struct reliefos_audio_format {
    uint32_t sample_rate;
    uint16_t channels;
    uint16_t bits_per_sample;
    uint32_t flags;
};

struct reliefos_audio_write {
    const void *data;
    uint32_t length;
    uint32_t transferred;
    uint32_t status;
};

struct reliefos_audio_state {
    uint32_t present;
    uint32_t active;
    uint32_t sample_rate;
    uint16_t channels;
    uint16_t bits_per_sample;
    uint32_t queued_bytes;
    uint32_t underruns;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
    uint8_t reserved;
};

#endif /* RELIEFOS_UAPI_AUDIO_ABI_H */
