#include QMK_KEYBOARD_H

#include "soft_ssd1306.h"
#include "hardware/structs/iobank0.h"

// Call this in keyboard_post_init_user or matrix_init_user
void disable_swd(void) {
    // Set SWCLK (GP24) and SWDIO (GP25) as normal inputs
    iobank0_hw->io[24].ctrl = 0x1F; // GPIO24
    iobank0_hw->io[25].ctrl = 0x1F; // GPIO25
}

enum layers {
    _BASE = 0,
    _FN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[_BASE] = LAYOUT_tkl(
/* ROW 0 */ KC_ESC, KC_NO, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_PSCR, KC_SCRL, KC_PAUSE,
/* ROW 1 */ KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_INS, KC_HOME, KC_PGUP,
/* ROW 2 */ KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, KC_END, KC_PGDN,
/* ROW 3 */ KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NO, KC_ENT, KC_NO, KC_NO, KC_NO,
/* ROW 4 */ KC_LSFT, KC_NO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_NO, KC_RSFT, KC_NO, KC_UP, KC_NO,
/* ROW 5 */ KC_LCTL, KC_LGUI, KC_LALT, KC_NO, KC_NO, KC_SPC, KC_NO, KC_NO, KC_RALT, KC_RGUI, KC_APP, KC_RCTL, TG(_FN), KC_NO, KC_LEFT, KC_DOWN, KC_RGHT
),

[_FN] = LAYOUT_tkl(
/* ROW 0 */ QK_RGB_MATRIX_TOGGLE, KC_NO, QK_RGB_MATRIX_VALUE_DOWN, QK_RGB_MATRIX_VALUE_UP, QK_RGB_MATRIX_SATURATION_DOWN, QK_RGB_MATRIX_SATURATION_UP, QK_RGB_MATRIX_HUE_DOWN, QK_RGB_MATRIX_HUE_UP, QK_RGB_MATRIX_SPEED_DOWN, QK_RGB_MATRIX_SPEED_UP, QK_RGB_MATRIX_MODE_PREVIOUS, QK_RGB_MATRIX_MODE_NEXT, KC_NO, KC_NO, KC_MPLY, KC_MPRV, KC_MNXT,
/* ROW 1 */ KC_GRV, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_MINS, KC_EQL, KC_BSPC, KC_MUTE, KC_VOLD, KC_VOLU,
/* ROW 2 */ KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL, KC_END, KC_PGDN,
/* ROW 3 */ KC_CAPS, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT, KC_NO, KC_ENT, KC_NO, KC_NO, KC_NO,
/* ROW 4 */ KC_LSFT, KC_NO, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_NO, KC_RSFT, KC_NO, KC_UP, KC_NO,
/* ROW 5 */ KC_LCTL, KC_LGUI, KC_LALT, KC_NO, KC_NO, KC_SPC, KC_NO, KC_NO, KC_RALT, KC_RGUI, KC_APP, KC_RCTL, TG(_FN), KC_NO, KC_LEFT, KC_DOWN, KC_RGHT
),

};


static uint8_t oled_current_layer = 0;
static char last_key = ' ';

void update_oled(void) {
    ssd1306_clear();
    char buf[20];
    
    // Line 1: Layer
    switch(oled_current_layer) {
       case _BASE: snprintf(buf, sizeof(buf), "Layer: BASE"); break;
       case _FN:   snprintf(buf, sizeof(buf), "Layer: FN");   break;
       default:    snprintf(buf, sizeof(buf), "Layer: OTHER"); break;
    }
    oled_print_line(0, buf);

    // Line 2: RGB ON/OFF + numeric brightness
    uint8_t brightness = rgb_matrix_get_val(); // get current brightness
    brightness = brightness / 255.0 * 100; 
    snprintf(buf, sizeof(buf), "RGB: %s %3d%%", 
    rgb_matrix_is_enabled() ? "ON" : "OFF", brightness);
    oled_print_line(1, buf);
}

void keyboard_post_init_user(void) {
    oled_init_soft();
    update_oled();
    
     // ----- CUSTOM ENCODER -----
    disable_swd();
}

layer_state_t layer_state_set_user(layer_state_t state) {
    oled_current_layer = biton32(state);
    update_oled();
    return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch(keycode) {
            case KC_A: last_key = 'A'; break;
            case KC_B: last_key = 'B'; break;
            case KC_C: last_key = 'C'; break;
            case KC_D: last_key = 'D'; break;
            case KC_E: last_key = 'E'; break;
            default: last_key = '?'; break;
        }
        //update_oled();
    }
    return true;
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state)) {

        case _FN:
            if (clockwise) {
                tap_code16(QK_RGB_MATRIX_VALUE_UP);
            } else {
                tap_code16(QK_RGB_MATRIX_VALUE_DOWN);
            }
            break;

        default: // _BASE
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
            break;
    }
    return false;
}


#ifdef RGB_MATRIX_ENABLE

bool rgb_matrix_indicators_user(void) {
    // ----------------------------
    // First 87 LEDs: keep animations
    // ----------------------------
    // Do nothing here, animations run automatically

    // ----------------------------
    // Last 6 LEDs (LEDs 87-92)
    // ----------------------------
    uint8_t r=0, g=0, b=0;    // default color

    // Layer-based color
    switch (get_highest_layer(layer_state)) {
        case _BASE:
            r = 50; g = 50; b = 50;  // dim red for BASE
            break;
        case _FN:
            r = 100; g = 0; b = 0; // medium red for FN
            break;
    }

    // Caps Lock overrides layer color
    if (host_keyboard_led_state().caps_lock) {
        r = 0; g = 0; b = 100;    // medium blue
    }

    // Apply to last 6 LEDs
    for (uint8_t i = 87; i < 93; i++) {
        rgb_matrix_set_color(i, r, g, b);
    }

    return true; // apply after animations
}

#endif
