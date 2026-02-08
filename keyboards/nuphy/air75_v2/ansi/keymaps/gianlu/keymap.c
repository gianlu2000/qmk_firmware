/*
Copyright 2023 @ Nuphy <https://nuphy.com/>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


// https://docs.qmk.fm/feature_macros

#include QMK_KEYBOARD_H

// Set the delay for sending characters in text macros
int text_char_delay = 30; // default: 8


// Define custom keycodes
enum gianlu_keycodes {

    // Macro Outlook
	MA_OBNG = SAFE_RANGE,
	MA_OBNS,
	MA_OCDC,
	MA_OJAB,
	MA_OJAS,
	MA_OCOF,
	MA_OCOS,
	MA_OBCH,
	MA_OBCS,

    // Macro Ticket
    MA_TRES,
    MA_TREP,
    MA_TNOT,
    MA_TNOR,
    MA_TNRR,
};

// Process custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {

		// Macro Outlook
        case MA_OBNG:
            if (record->event.pressed) {
                // when keycode MA_OBNG is pressed
                SEND_STRING_DELAY("Buongiorno,\n", text_char_delay);
            } else {
                // when keycode MA_OBNG is released
            }
            break;

        case MA_OBNS:
            if (record->event.pressed) {
                // when keycode MA_OBNS is pressed
                SEND_STRING_DELAY("Buonasera,\n", text_char_delay);
            } else {
                // when keycode MA_OBNS is released
            }
            break;

        case MA_OCDC:
            if (record->event.pressed) {
                // when keycode MA_OCDC is pressed
                SEND_STRING_DELAY("come da contatto ", text_char_delay);
            } else {
                // when keycode MA_OCDC is released
            }
            break;

        case MA_OJAB:
            if (record->event.pressed) {
                // when keycode MA_OJAB is pressed
                SEND_STRING_DELAY("si notifica la seguente fase in abend\n\n", text_char_delay);
            } else {
                // when keycode MA_OJAB is released
            }
            break;

        case MA_OJAS:
            if (record->event.pressed) {
                // when keycode MA_OJAS is pressed
                SEND_STRING_DELAY("si notifica le seguenti fasi in abend\n\n", text_char_delay);
            } else {
                // when keycode MA_OJAS is released
            }
            break;

        case MA_OCOF:
            if (record->event.pressed) {
                // when keycode MA_OCOF is pressed
				// TODO: CONTINUA DA QUI
                SEND_STRING_DELAY("si notifica il ritardo del cut-off in oggetto al seguente CP\n\n", text_char_delay);
            } else {
                // when keycode MA_OCOF is released
            }
            break;

        case MA_OCOS:
            if (record->event.pressed) {
                // when keycode MA_OCOS is pressed
                SEND_STRING_DELAY("si notifica il ritardo dei seguenti cut-off ai CP indicati\n\n", text_char_delay);
            } else {
                // when keycode MA_OCOS is released
            }
            break;

        case MA_OBCH:
            if (record->event.pressed) {
                // when keycode MA_OBCH is pressed
                SEND_STRING_DELAY("si notifica il ritardo del giro in oggetto al seguente CP\n\n", text_char_delay);
            } else {
                // when keycode MA_OBCH is released
            }
            break;

        case MA_OBCS:
            if (record->event.pressed) {
                SEND_STRING_DELAY("si notifica il ritardo dei seguenti giri ai CP indicati\n\n", text_char_delay);
            } else {
                // when keycode MA_OBCS is released
            }
            break;

        // Macro Ticket
        case MA_TRES:
            if (record->event.pressed) {
                // when keycode MA_TRES is pressed
                SEND_STRING_DELAY("Restartato", text_char_delay);
            } else {
                // when keycode MA_TRES is released
            }
            break;

        case MA_TREP:
            if (record->event.pressed) {
                // when keycode MA_TREP is pressed
                SEND_STRING_DELAY("Restartato come da prosa", text_char_delay);
            } else {
                // when keycode MA_TREP is released
            }
            break;

        case MA_TNOT:
            if (record->event.pressed) {
                // when keycode MA_TNOT is pressed
                SEND_STRING_DELAY("Notificato", text_char_delay);
            } else {
                // when keycode MA_TNOT is released
            }
            break;
            
        case MA_TNOR:
            if (record->event.pressed) {
                // when keycode MA_TNOR is pressed
                SEND_STRING_DELAY("Notificato al reperibile", text_char_delay);
            } else {
                // when keycode MA_TNOR is released
            }
            break;

        case MA_TNRR:
            if (record->event.pressed) {
                // when keycode MA_TNRR is pressed
                SEND_STRING_DELAY("Notificato al reperibile e restartato su sua richiesta", text_char_delay);
            } else {
                // when keycode MA_TNRR is released
            }
            break;

    }
    return true;
}


// Define layers
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// layer Mac
[0] = LAYOUT_ansi_84(
	KC_ESC, 	KC_BRID,  	KC_BRIU,  	MAC_TASK, 	MAC_SEARCH, MAC_VOICE,  MAC_DND,  	KC_MPRV,  	KC_MPLY,  	KC_MNXT, 	KC_MUTE, 	KC_VOLD, 	KC_VOLU, 	MAC_PRTA,	KC_INS,		KC_DEL,
	KC_GRV, 	KC_1,   	KC_2,   	KC_3,  		KC_4,   	KC_5,   	KC_6,   	KC_7,   	KC_8,   	KC_9,  		KC_0,   	KC_MINS,	KC_EQL, 				KC_BSPC,	KC_PGUP,
	KC_TAB, 	KC_Q,   	KC_W,   	KC_E,  		KC_R,   	KC_T,   	KC_Y,   	KC_U,   	KC_I,   	KC_O,  		KC_P,   	KC_LBRC,	KC_RBRC, 				KC_BSLS,	KC_PGDN,
	KC_CAPS,	KC_A,   	KC_S,   	KC_D,  		KC_F,   	KC_G,   	KC_H,   	KC_J,   	KC_K,   	KC_L,  		KC_SCLN,	KC_QUOT, 	 						KC_ENT,		KC_HOME,
	KC_LSFT,				KC_Z,   	KC_X,   	KC_C,  		KC_V,   	KC_B,   	KC_N,   	KC_M,   	KC_COMM,	KC_DOT,		KC_SLSH,				KC_RSFT,	KC_UP,		KC_END,
	KC_LCTL,	KC_LALT,	KC_LGUI,										KC_SPC, 							KC_RGUI,	MO(1),   	KC_RCTL,				KC_LEFT,	KC_DOWN,    KC_RGHT),

// layer Mac Fn
[1] = LAYOUT_ansi_84(
	_______, 	KC_F1,  	KC_F2,  	KC_F3, 		KC_F4,  	KC_F5,  	KC_F6,  	KC_F7,  	KC_F8,  	KC_F9, 		KC_F10, 	KC_F11, 	KC_F12, 	MAC_PRT,	_______,	_______,
	_______, 	LNK_BLE1,  	LNK_BLE2,  	LNK_BLE3,  	LNK_RF,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	_______, 				_______,	_______,
	_______, 	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	DEV_RESET,	SLEEP_MODE, 			BAT_SHOW,	_______,
	_______,	_______,   	_______,   	_______,  	_______,   	_______,   	_______,	_______,   	_______,   	_______,  	_______,	_______, 	 						_______,	_______,
	_______,				_______,   	_______,   	RGB_TEST,  	_______,   	BAT_NUM,   	_______,	MO(4), 		RGB_SPD,	RGB_SPI,	_______,				_______,	RGB_VAI,	_______,
	_______,	_______,	_______,										_______, 							_______,	MO(1),   	_______,				RGB_MOD,	RGB_VAD,    RGB_HUI),

// layer win
[2] = LAYOUT_ansi_84(
	KC_ESC, 	KC_F1,  	KC_F2,  	KC_F3, 		KC_F4,  	KC_F5,  	KC_F6,  	KC_F7,  	KC_F8,  	KC_F9, 		KC_F10, 	KC_F11, 	KC_F12, 	KC_PSCR,	KC_INS,		KC_DEL,
	KC_GRV, 	KC_1,   	KC_2,   	KC_3,  		KC_4,   	KC_5,   	KC_6,   	KC_7,   	KC_8,   	KC_9,  		KC_0,   	KC_MINS,	KC_EQL, 				KC_BSPC,	KC_PGUP,
	KC_TAB, 	KC_Q,   	KC_W,   	KC_E,  		KC_R,   	KC_T,   	KC_Y,   	KC_U,   	KC_I,   	KC_O,  		KC_P,   	KC_LBRC,	KC_RBRC, 				KC_BSLS,	KC_PGDN,
	KC_CAPS,	KC_A,   	KC_S,   	KC_D,  		KC_F,   	KC_G,   	KC_H,   	KC_J,   	KC_K,   	KC_L,  		KC_SCLN,	KC_QUOT, 	 						KC_ENT,		KC_HOME,
	KC_LSFT,				KC_Z,   	KC_X,   	KC_C,  		KC_V,   	KC_B,   	KC_N,   	KC_M,   	KC_COMM,	KC_DOT,		KC_SLSH,				KC_RSFT,	KC_UP,		KC_END,
	KC_LCTL,	KC_LGUI,	KC_LALT,										KC_SPC, 							KC_RALT,	MO(3),   	KC_RCTL,				KC_LEFT,	KC_DOWN,    KC_RGHT),

// layer win Fn
[3] = LAYOUT_ansi_84(
	_______, 	KC_BRID,   	KC_BRIU,    _______,  	_______,   	_______,   	_______,   	KC_MPRV,   	KC_MPLY,   	KC_MNXT,  	KC_MUTE, 	KC_VOLD, 	KC_VOLU,	_______,	_______,	_______,
	_______, 	LNK_BLE1,  	LNK_BLE2,  	LNK_BLE3,  	LNK_RF,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	_______, 				_______,	_______,
	_______, 	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	MO(5),  	MO(6),     	DEV_RESET,	SLEEP_MODE, 			BAT_SHOW,	_______,
	_______,	_______,   	_______,   	_______,  	_______,   	_______,   	_______,	_______,   	_______,   	_______,  	_______,	_______, 	 						_______,	_______,
	_______,				_______,   	_______,   	RGB_TEST,  	_______,   	BAT_NUM,   	_______,	MO(4), 		RGB_SPD,	RGB_SPI,	_______,				_______,	RGB_VAI,	_______,
	_______,	_______,	_______,										_______, 							_______,	MO(3),   	_______,				RGB_MOD,	RGB_VAD,    RGB_HUI),

// layer 4
[4] = LAYOUT_ansi_84(
	_______, 	_______,  	_______,  	_______, 	_______,  	_______,  	_______,  	_______,  	_______,  	_______, 	_______, 	_______, 	_______, 	_______,	_______,	_______,
	_______, 	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	_______, 				_______,	_______,
	_______, 	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	_______, 				_______,	_______,
	_______,	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,	_______, 	 						_______,	_______,
	_______,				_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	SIDE_SPD,	SIDE_SPI,	_______,				_______,	SIDE_VAI,	_______,
	_______,	_______,	_______,										_______, 							_______,	MO(4),   	_______,				SIDE_MOD,	SIDE_VAD,   SIDE_HUI),

// layer 5 - Outlook
[5] = LAYOUT_ansi_84(
	_______, 	_______,  	_______,  	_______, 	_______,  	_______,  	_______,  	_______,  	_______,  	_______, 	_______, 	_______, 	_______, 	_______,	_______,	_______,
	_______, 	MA_OBNG,   	MA_OCDC,   	MA_OJAB,  	MA_OCOF,   	MA_OBCH,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	_______, 				_______,	_______,
	_______, 	MA_OBNS,   	_______,   	MA_OJAS,  	MA_OCOS,   	MA_OBCS,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	_______, 				_______,	_______,
	_______,	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,	_______, 	 						_______,	_______,
	_______,				_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,	_______,	_______,				_______,	_______,	_______,
	_______,	_______,	_______,										_______, 							_______,	MO(5),   	_______,				_______,	_______,   _______),

// layer 6 - Ticket
[6] = LAYOUT_ansi_84(
	_______, 	_______,  	_______,  	_______, 	_______,  	_______,  	_______,  	_______,  	_______,  	_______, 	_______, 	_______, 	_______, 	_______,	_______,	_______,
	_______, 	MA_TRES,   	MA_TNOT,   	MA_TNRR,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	_______, 				_______,	_______,
	_______, 	MA_TREP,   	MA_TNOR,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,   	_______,	_______, 				_______,	_______,
	_______,	_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,   	_______,  	_______,	_______, 	 						_______,	_______,
	_______,				_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,	_______,	_______,				_______,	_______,	_______,
	_______,	_______,	_______,										_______, 							_______,	MO(6),   	_______,				_______,	_______,   _______)
};
