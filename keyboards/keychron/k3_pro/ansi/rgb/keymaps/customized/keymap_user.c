/* Copyright 2023 @ Muhan Li
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "keymap_matrix.c"

#include "keymap_user.h"

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // keyboard locked
    if (secure_is_locked()) {
        secure_request_unlock();
        return false;
    }

    // unlocked
    switch (keycode) {
    case MY_LANG_LOCK:
        if (record->event.pressed) {
            indicator_activate(&MY_LANG_LOCK_TAPPED);
        } else {
            if (indicator_is_active(&MY_LANG_LOCK_TAPPED)) {
                indicator_deactivate(&MY_LANG_LOCK_TAPPED);
                tap_code(KC_CAPS);
            } else {
                lock_system_and_keyboard();
            }
        }
        break;
    case MY_MO_LSPO:
        if (record->event.pressed) {
            indicator_activate(&MY_MO_LSPO_TAPPED);
            register_code(KC_LSFT);
        } else {
            // Left Shift when held, ( when tapped
            unregister_code(KC_LSFT);
            if (indicator_is_tapped(&MY_MO_LSPO_TAPPED)) {
                tap_code16(S(KC_9)); // (
            }
        }
        break;
    case MY_MO_RCPC:
        if (record->event.pressed) {
            indicator_activate(&MY_MO_RCPC_TAPPED);
            layer_on(layer_state_is(L_CAPS) ? L_BOTH : L_RSFT);
        } else {
            // Right Control when held, ) when tapped
            layer_off(layer_state_is(L_BOTH) ? L_BOTH : L_RSFT);
            if (indicator_is_tapped(&MY_MO_RCPC_TAPPED)) {
                tap_code16(S(KC_0)); // )
            }
        }
        break;
    default:
        // suppress Space Cadet if any other key been pressed
        indicator_deactivate(&MY_MO_LSPO_TAPPED);
        indicator_deactivate(&MY_MO_RCPC_TAPPED);
        // Process all other keycodes normally
        return true;
    }
    return false;  // Skip all further processing of this key
}

// auto shift functions
bool get_custom_auto_shifted_key(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
    // SPECIAL except TAB
    case KC_MINUS ... KC_SLASH:
    case KC_NONUS_BACKSLASH:
        return true;
    default:
        return false;
    }
}

uint16_t get_autoshift_timeout(uint16_t keycode, keyrecord_t *record) {
    switch(keycode) {
    case AUTO_SHIFT_NUMERIC:
        return get_generic_autoshift_timeout() + 100;
    case AUTO_SHIFT_SPECIAL:
        return get_generic_autoshift_timeout() + 30;
    case AUTO_SHIFT_ALPHA:
    default:
        return get_generic_autoshift_timeout();
    }
}

// leader key functions
void leader_start_user(void) {
    // Do something when the leader key is pressed
}

// TODO
void leader_end_user(void) {
    if (leader_sequence_one_key(KC_F)) {
        // Leader, f => Types the below string
        SEND_STRING("QMK is awesome.");
    } else if (leader_sequence_two_keys(KC_D, KC_D)) {
        // Leader, d, d => Ctrl+A, Ctrl+C
        SEND_STRING(SS_LCTL("a") SS_LCTL("c"));
    } else if (leader_sequence_three_keys(KC_D, KC_D, KC_S)) {
        // Leader, d, d, s => Types the below string
        SEND_STRING("https://start.duckduckgo.com\n");
    } else if (leader_sequence_two_keys(KC_A, KC_S)) {
        // Leader, a, s => GUI+S
        tap_code16(LGUI(KC_S));
    } else if (leader_sequence_two_keys(KC_O, KC_S)) {
        // Leader, o, s => print os
        switch(detected_host_os()) {
        case OS_MACOS:
        case OS_IOS:
            SEND_STRING("Apple");
            break;
        case OS_WINDOWS:
            SEND_STRING("Windows");
            break;
        case OS_LINUX:
            SEND_STRING("Linux");
            break;
        default:
            SEND_STRING("unknown");
        }
    }
}
