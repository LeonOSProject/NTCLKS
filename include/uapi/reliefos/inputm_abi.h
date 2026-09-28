#ifndef RELIEFOS_UAPI_INPUTM_ABI_H
#define RELIEFOS_UAPI_INPUTM_ABI_H
/*
 * Input-method service wire ABI between the inputm service and userland.
 * Userland wrappers live in <reliefos/inputm.h>.
 * UAPI only: nothing here may include a non-UAPI header.
 */

#include <stdint.h>


#define RELIEFOS_INPUTM_MAX_PROVIDERS 8U
#define RELIEFOS_INPUTM_MAX_CANDIDATES 5U
#define RELIEFOS_INPUTM_ID_LEN 32U
#define RELIEFOS_INPUTM_NAME_LEN 64U
#define RELIEFOS_INPUTM_ABBREV_LEN 8U
#define RELIEFOS_INPUTM_TEXT_LEN 128U

#define RELIEFOS_INPUTM_START_MANUAL 0U
#define RELIEFOS_INPUTM_START_LOGIN 1U
#define RELIEFOS_INPUTM_START_ON_DEMAND 2U

#define RELIEFOS_INPUTM_CONTEXT_FOCUSED 0x00000001U
#define RELIEFOS_INPUTM_CONTEXT_SECURE 0x00000002U

#define RELIEFOS_INPUTM_RESULT_COMPOSITION 1U
#define RELIEFOS_INPUTM_RESULT_COMMIT 2U
#define RELIEFOS_INPUTM_RESULT_CANCEL 3U
#define RELIEFOS_INPUTM_RESULT_PASSTHROUGH 4U

#define RELIEFOS_INPUTM_RENDER_CONTROLS 0x00000001U
#define RELIEFOS_INPUTM_RENDER_PIXELS 0x00000002U

struct reliefos_inputm_provider {
    char id[RELIEFOS_INPUTM_ID_LEN];
    char name[RELIEFOS_INPUTM_NAME_LEN];
    char abbreviation[RELIEFOS_INPUTM_ABBREV_LEN];
    uint32_t startup_mode;
    uint32_t render_flags;
    uint32_t enabled;
};

struct reliefos_inputm_key_event {
    uint32_t sequence;
    uint32_t client_pid;
    uint32_t window_id;
    uint32_t context_flags;
    uint8_t keycode;
    uint8_t pressed;
    uint8_t reserved0;
    uint8_t reserved1;
    int32_t caret_x;
    int32_t caret_y;
    uint32_t caret_w;
    uint32_t caret_h;
};

struct reliefos_inputm_result {
    uint32_t sequence;
    uint32_t client_pid;
    uint32_t window_id;
    uint32_t type;
    char text[RELIEFOS_INPUTM_TEXT_LEN];
    char candidates[RELIEFOS_INPUTM_MAX_CANDIDATES][RELIEFOS_INPUTM_TEXT_LEN];
    uint32_t candidate_count;
    uint32_t selected_candidate;
    uint8_t keycode;
    uint8_t pressed;
    uint8_t reserved0;
    uint8_t reserved1;
};

struct reliefos_inputm_active_request {
    uint32_t uid;
    char id[RELIEFOS_INPUTM_ID_LEN];
};

struct reliefos_inputm_config_request {
    uint32_t uid;
};

struct reliefos_inputm_provider_list {
    uint32_t uid;
    uint32_t capacity;
    uint32_t count;
    uint32_t reserved;
    struct reliefos_inputm_provider *providers;
};

struct reliefos_inputm_context {
    uint32_t window_id;
    uint32_t flags;
    int32_t caret_x;
    int32_t caret_y;
    uint32_t caret_w;
    uint32_t caret_h;
};

struct reliefos_inputm_state {
    uint32_t uid;
    char active_id[RELIEFOS_INPUTM_ID_LEN];
    char composition[RELIEFOS_INPUTM_TEXT_LEN];
    char candidates[RELIEFOS_INPUTM_MAX_CANDIDATES][RELIEFOS_INPUTM_TEXT_LEN];
    uint32_t candidate_count;
    uint32_t selected_candidate;
    uint32_t render_flags;
    uint32_t config_generation;
    uint32_t window_id;
    int32_t caret_x;
    int32_t caret_y;
    uint32_t caret_w;
    uint32_t caret_h;
};

#endif /* RELIEFOS_UAPI_INPUTM_ABI_H */
