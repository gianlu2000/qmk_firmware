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
#include <ctype.h>
#include <string.h>

// Set the delay for sending characters in text macros
int text_char_delay = 30; // default: 8

static bool macro_running = false; // flag per indicare se una macro è in esecuzione
static uint8_t macro_index = 0; // per tenere traccia del keycode della macro in esecuzione
static uint16_t macro_timer = 0; // per gestire il timing della macro
static const char *macro_str = NULL; // stringa della macro in esecuzione
static uint16_t macro_len = 0; // lunghezza della stringa della macro
static int macro_led_index = -1; // indice LED della key che ha avviato la macro
extern led_config_t g_led_config;


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
    MA_TFGC,
};

// Process custom keycodes
// Quando viene premuto un tasto che corrisponde a una macro, chiamiamo start_macro con la stringa da inviare e l'indice del LED da accendere. La funzione start_macro si occupa di inizializzare lo stato della macro e di accendere il LED corrispondente. Il processo di invio dei caratteri della macro avviene in matrix_scan_user, che controlla periodicamente se è il momento di inviare il prossimo carattere senza bloccare il loop principale di QMK.
static void start_macro(const char *s, int led_index);
static void get_led_color_for_index(uint8_t idx, uint8_t *r, uint8_t *g, uint8_t *b);

// This function is called on every key event. We use it to trigger our macros and manage the macro state.
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Se una macro è in esecuzione, blocca tutti i tasti tranne ESC (stop emergenza)
    if (macro_running) {
        if (keycode == KC_ESC) {
            if (record->event.pressed) {
                macro_running = false;
                macro_str = NULL;
                macro_len = 0;
                macro_index = 0;
                macro_led_index = -1;
                rgb_matrix_reload_from_eeprom();
            }
            return false; // consuma l'evento ESC (serve solo per lo stop)
        }
        if (record->event.pressed) {
            return false; // blocca qualsiasi altro tasto mentre la macro è in corso
        }
    }
    switch (keycode) {

		// Macro Outlook
        case MA_OBNG:
            if (record->event.pressed) {
                start_macro("Buongiorno,\n", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_OBNS:
            if (record->event.pressed) {
                start_macro("Buonasera,\n", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_OCDC:
            if (record->event.pressed) {
                start_macro("come da contatto ", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_OJAB:
            if (record->event.pressed) {
                start_macro("si notifica la seguente fase in abend\n\n", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_OJAS:
            if (record->event.pressed) {
                start_macro("si notificano le seguenti fasi in abend\n\n", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_OCOF:
            if (record->event.pressed) {
                start_macro("si notifica il ritardo del cut-off in oggetto al seguente CP\n\n", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_OCOS:
            if (record->event.pressed) {
                start_macro("si notificano i ritardi dei seguenti cut-off ai CP indicati\n\n", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_OBCH:
            if (record->event.pressed) {
                start_macro("si notifica il ritardo del giro in oggetto al seguente CP\n\n", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_OBCS:
            if (record->event.pressed) {
                start_macro("si notificano i ritardi dei seguenti giri ai CP indicati\n\n", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        // Macro Ticket
        case MA_TRES:
            if (record->event.pressed) {
                start_macro("Restartato", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_TREP:
            if (record->event.pressed) {
                start_macro("Restartato come da prosa", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_TNOT:
            if (record->event.pressed) {
                start_macro("Notificato", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;
            
        case MA_TNOR:
            if (record->event.pressed) {
                start_macro("Notificato al reperibile", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_TNRR:
            if (record->event.pressed) {
                start_macro("Notificato al reperibile e restartato su sua richiesta", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

        case MA_TFGC:
            if (record->event.pressed) {
                start_macro("Fase da completare già in complete, fase completata", g_led_config.matrix_co[record->event.key.row][record->event.key.col]);
            }
            break;

    }
    return true;
}

// --- Non-blocking macro engine ---
// Questa è un'implementazione molto semplice che invia i caratteri di una stringa uno alla volta ogni text_char_delay millisecondi, senza bloccare il loop principale di QMK. Supporta anche alcuni caratteri Unicode comuni (come le lettere accentate italiane) usando sequenze di tasti con dead-key. Puoi espandere la funzione send_unicode_nonblocking per supportare più caratteri se necessario.
static void start_macro(const char *s, int led_index) {
    if (!s) return;
    macro_str = s;
    macro_len = (uint16_t)strlen(s);
    macro_index = 0;
    macro_timer = timer_read();
    macro_running = true;

    if (led_index >= 0 && led_index < RGB_MATRIX_LED_COUNT) {
        macro_led_index = led_index;
    } else {
        macro_led_index = -1;
    }

    // Metti in modalità colore fisso e mostra solo il LED della macro e quello di ESC
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    for (uint16_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
        rgb_matrix_set_color(i, 0, 0, 0);
    }
    // indice LED del tasto ESC (prima riga, prima colonna)
    int esc_led_index = g_led_config.matrix_co[0][0];   // definisci l'indice del LED di ESC
    if (esc_led_index != NO_LED && esc_led_index >= 0 && esc_led_index < RGB_MATRIX_LED_COUNT) {
        rgb_matrix_set_color((uint8_t)esc_led_index, 255, 0, 0);
    }
    if (macro_led_index >= 0) {
        // Se il tasto delle macro ha l'index maggiore di 0, accendi il LED corrispondente
        // accendi il LED del tasto che ha avviato la macro con un colore definito in RGB

        uint8_t mr = 255, mg = 160, mb = 0;
        get_led_color_for_index((uint8_t)macro_led_index, &mr, &mg, &mb);
        rgb_matrix_set_color((uint8_t)macro_led_index, mr, mg, mb);
    }
}

// Invia un singolo carattere senza bloccare. Supporta solo un sottoinsieme di caratteri ASCII e alcuni simboli comuni. Puoi espandere questa funzione per supportare più caratteri se necessario.
static void send_char_nonblocking(char c) {
    if (c == '\n') { tap_code(KC_ENT); return; }
    if (c == ' ') { tap_code(KC_SPC); return; }
    if (c == ',') { tap_code(KC_COMM); return; }
    if (c == '.') { tap_code(KC_DOT); return; }
    if (c == '\'') { tap_code(KC_QUOT); return; }
    if (c == '-') { tap_code(KC_MINS); return; }
    if (c >= '0' && c <= '9') { tap_code(KC_0 + (c - '0')); return; }
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        bool upper = (c >= 'A' && c <= 'Z');
        uint8_t kc = KC_A + (uint8_t)(tolower(c) - 'a');
        if (upper) {
            register_code(KC_LSFT);
            tap_code(kc);
            unregister_code(KC_LSFT);
        } else {
            tap_code(kc);
        }
        return;
    }
    // fallback: ignore unknown chars
}

// This is a very basic implementation that only handles a few common Unicode characters.
// Puoi espandere questa funzione per supportare più caratteri Unicode se necessario. L'idea è di mappare i caratteri Unicode che ti interessano a sequenze di tasti che funzionano con il layout della tastiera del tuo computer (ad esempio, usando dead-key per le lettere accentate italiane su layout US-International).
static void send_unicode_nonblocking(uint16_t cp) {
    // Map common Italian accented letters using dead-keys on
    // US-International host layout.
    // Grave accents: ` + letter
    // Acute accents: '  + letter (sent as KC_QUOT)

    // à / À
    if (cp == 0x00E0) { tap_code(KC_GRV); wait_ms(20); tap_code(KC_A); return; }
    if (cp == 0x00C0) { tap_code(KC_GRV); wait_ms(20); register_code(KC_LSFT); tap_code(KC_A); unregister_code(KC_LSFT); return; }

    // è / È  (grave)
    if (cp == 0x00E8) { tap_code(KC_GRV); wait_ms(20); tap_code(KC_E); return; }
    if (cp == 0x00C8) { tap_code(KC_GRV); wait_ms(20); register_code(KC_LSFT); tap_code(KC_E); unregister_code(KC_LSFT); return; }

    // é / É  (acute)
    if (cp == 0x00E9) { tap_code(KC_QUOT); wait_ms(20); tap_code(KC_E); return; }
    if (cp == 0x00C9) { tap_code(KC_QUOT); wait_ms(20); register_code(KC_LSFT); tap_code(KC_E); unregister_code(KC_LSFT); return; }

    // ì / Ì (grave)
    if (cp == 0x00EC) { tap_code(KC_GRV); wait_ms(20); tap_code(KC_I); return; }
    if (cp == 0x00CC) { tap_code(KC_GRV); wait_ms(20); register_code(KC_LSFT); tap_code(KC_I); unregister_code(KC_LSFT); return; }

    // ò / Ò (grave)
    if (cp == 0x00F2) { tap_code(KC_GRV); wait_ms(20); tap_code(KC_O); return; }
    if (cp == 0x00D2) { tap_code(KC_GRV); wait_ms(20); register_code(KC_LSFT); tap_code(KC_O); unregister_code(KC_LSFT); return; }

    // ù / Ù (grave)
    if (cp == 0x00F9) { tap_code(KC_GRV); wait_ms(20); tap_code(KC_U); return; }
    if (cp == 0x00D9) { tap_code(KC_GRV); wait_ms(20); register_code(KC_LSFT); tap_code(KC_U); unregister_code(KC_LSFT); return; }

    // fallback: ignore unknown unicode
}


// Call this in matrix_scan_user to handle sending macro characters over time without blocking the main loop.
void matrix_scan_user(void) {
    if (!macro_running || !macro_str) return;
    if (timer_elapsed(macro_timer) >= (uint16_t)text_char_delay) {
        uint8_t b = (uint8_t)macro_str[macro_index];

        // Check if this is the start of a UTF-8 sequence for a character we want to support
        if (b >= 0xC2 && (macro_index + 1) < macro_len) {
            // assume a 2-byte UTF-8 sequence (Latin-1 supplements like à)
            uint8_t b2 = (uint8_t)macro_str[macro_index + 1];
            uint16_t cp = ((b & 0x1F) << 6) | (b2 & 0x3F);
            macro_index += 2;
            send_unicode_nonblocking(cp);
        } else {
            // Regular ASCII character
            char c = macro_str[macro_index++];
            send_char_nonblocking(c);
        }
        macro_timer = timer_read();
        if (macro_index >= macro_len) {
            macro_running = false;
            macro_str = NULL;
            macro_len = 0;
            macro_index = 0;
            macro_led_index = -1;
            rgb_matrix_reload_from_eeprom();
        }
    }
}


// Define layers
// Vengono definiti i vari layer, con il layer 0 che è quello principale per Mac, il layer 1 per le funzioni Fn su Mac, il layer 2 per Windows, il layer 3 per le funzioni Fn su Windows, e i layer 4-6 per le macro di Outlook e Ticket. Ogni tasto è mappato a una funzione specifica, e i tasti che attivano le macro sono evidenziati con i colori corrispondenti nei layer Fn.
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
	_______,	_______,   	_______,   	_______,  	_______,   	MA_TFGC,   	_______,   	_______,   	_______,   	_______,  	_______,	_______, 	 						_______,	_______,
	_______,				_______,   	_______,   	_______,  	_______,   	_______,   	_______,   	_______,   	_______,	_______,	_______,				_______,	_______,	_______,
	_______,	_______,	_______,										_______, 							_______,	MO(6),   	_______,				_______,	_______,   _______)
};


// Metti in pausa l’effetto quando entri nel layer 5 o 6, e ripristinalo quando esci
layer_state_t layer_state_set_user(layer_state_t state) {
    if (layer_state_cmp(state, 5) || layer_state_cmp(state, 6)) { // se sei in uno dei layer Fn
        
        // Pausa l’effetto e spegni i LED
        rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);

        rgb_matrix_sethsv_noeeprom(0, 0, 0); // base spento

    } else {
        rgb_matrix_reload_from_eeprom(); // ripristina effetto precedente
    }
    return state;
}



// tool per prendere i colori: https://www.rapidtables.com/web/color/RGB_Color.html

// RGB Matrix indicators for macros and layer states
// Restituisce il colore assegnato a un indice LED in base allo stato dei layer
static void get_led_color_for_index(uint8_t idx, uint8_t *r, uint8_t *g, uint8_t *b) {
    // fallback: arancione
    *r = 255; *g = 160; *b = 0;

    if (layer_state_is(5)) {
        // breathing value usato per il tasto O nel layer 5
        uint16_t t = timer_read();
        uint8_t phase = (t >> 3);
        uint8_t breathe;
        if (phase & 0x80) {
            breathe = 255 - ((phase & 0x7F) << 1);
        } else {
            breathe = (phase & 0x7F) << 1;
        }

        switch (idx) {
            case 29: *r = 255; *g = 0;   *b = 0;   break; // 1
            case 28: *r = 255; *g = 255; *b = 0;   break; // 2
            case 27: *r = 0;   *g = 255; *b = 0;   break; // 3
            case 26: *r = 0;   *g = 0;   *b = 255; break; // 4
            case 25: *r = 127; *g = 0;   *b = 255; break; // 5

            case 32: *r = 255; *g = 0;   *b = 0;   break; // Q
            case 34: *r = 0;   *g = 255; *b = 0;   break; // E
            case 35: *r = 0;   *g = 0;   *b = 255; break; // R
            case 36: *r = 127; *g = 0;   *b = 255; break; // T

            case 40: *r = 0;   *g = 0;   *b = breathe; break; // O (breathing)
            default: break;
        }
        return;
    } else if (layer_state_is(6)) {
        // breathing value usato per il tasto P nel layer 6 (ridotto 60%)
        uint16_t t = timer_read();
        uint8_t phase = (t >> 3);
        uint8_t breathe;
        if (phase & 0x80) {
            breathe = 255 - ((phase & 0x7F) << 1);
        } else {
            breathe = (phase & 0x7F) << 1;
        }
        breathe = (breathe * 153) / 255; // ~60%

        switch (idx) {
            case 29: *r = 0;   *g = 204; *b = 0; break; // 1
            case 28: *r = 0;   *g = 204; *b = 0; break; // 2
            case 27: *r = 0;   *g = 204; *b = 0; break; // 3

            case 32: *r = 0;   *g = 204; *b = 0; break; // Q
            case 33: *r = 0;   *g = 204; *b = 0; break; // W
            case 54: *r = 0;   *g = 204; *b = 0; break; // G

            case 41: *r = 0;   *g = breathe; *b = 0; break; // P (breathing)
            default: break;
        }
        return;
    }
    // altrimenti rimane il fallback
}

bool rgb_matrix_indicators_user(void) {
    // Se una macro è in esecuzione, mostra solo il LED della macro e spegni gli altri. Altrimenti, se sei nei layer Fn, mostra i colori specifici per quei layer. Se non sei in nessuno dei layer Fn, lascia tutto come è (effetti normali).

    if (macro_running) {
        // spegni tutti i LED e accendi solo ESC + quello della macro in corso (lampeggiante)
        for (uint16_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
        int esc_led = g_led_config.matrix_co[0][0];
        if (esc_led != NO_LED && esc_led >= 0 && esc_led < RGB_MATRIX_LED_COUNT) {
            rgb_matrix_set_color((uint8_t)esc_led, 255, 0, 0);
        }
        if (macro_led_index >= 0 && macro_led_index < RGB_MATRIX_LED_COUNT) {
            uint8_t mr = 255, mg = 160, mb = 0;
            get_led_color_for_index((uint8_t)macro_led_index, &mr, &mg, &mb);

            // Semlice lampeggio on/off (periodo 600ms -> 300ms on, 300ms off)
            uint16_t t = timer_read();
            bool on = (((t / 300) & 1) == 0);
            if (on) {
                rgb_matrix_set_color((uint8_t)macro_led_index, mr, mg, mb);
            } else {
                rgb_matrix_set_color((uint8_t)macro_led_index, 0, 0, 0);
            }
        }
        return false;
    }
    if (layer_state_is(5)) {
        // se sei nel layer Fn di Outlook, mostra i colori specifici per quel layer e un effetto di respiro sul tasto O

        // Effetto respiro semplice (onda triangolare)

        uint16_t t = timer_read();
        uint8_t phase = (t >> 3);   // velocità (più grande = più lento)

        uint8_t breathe;

        if (phase & 0x80) {
            breathe = 255 - ((phase & 0x7F) << 1);
        } else {
            breathe = (phase & 0x7F) << 1;
        }


        // Tasti colorati per layer Outlook
        rgb_matrix_set_color(29, 255, 0, 0);   // 1
        rgb_matrix_set_color(28, 255, 255, 0);   // 2
        rgb_matrix_set_color(27, 0, 255, 0);   // 3
        rgb_matrix_set_color(26, 0, 0, 255);   // 4
        rgb_matrix_set_color(25, 127, 0, 255);   // 5

        rgb_matrix_set_color(32, 255, 0, 0);   // Q
        // rgb_matrix_set_color(33, 255, 0, 0);   // W  - tasto non utilizzato per macro di testo, lo lascio spento
        rgb_matrix_set_color(34, 0, 255, 0);   // E
        rgb_matrix_set_color(35, 0, 0, 255);   // R
        rgb_matrix_set_color(36, 127, 0, 255);   // T

        // Breathing light effect
        rgb_matrix_set_color(40, 0, 0, breathe);   // Tasto O

    } else if (layer_state_is(6)) {
        // se sei nel layer Fn di Ticket, mostra i colori specifici per quel layer e un effetto di respiro sul tasto P

        uint16_t t = timer_read();
        uint8_t phase = (t >> 3);   // velocità (più grande = più lento)

        uint8_t breathe;

        if (phase & 0x80) {
            breathe = 255 - ((phase & 0x7F) << 1);
        } else {
            breathe = (phase & 0x7F) << 1;
        }

        breathe = (breathe * 153) / 255; // riduci la luminosità al 60% (153/255)

        // Tasti colorati per layer Ticket
        rgb_matrix_set_color(29, 0, 204, 0);   // 1
        rgb_matrix_set_color(28, 0, 204, 0);   // 2
        rgb_matrix_set_color(27, 0, 204, 0);   // 3

        rgb_matrix_set_color(32, 0, 204, 0);   // Q
        rgb_matrix_set_color(33, 0, 204, 0);   // W
        rgb_matrix_set_color(54, 0, 204, 0);   // G

        // Breathing light effect
        rgb_matrix_set_color(41, 0, breathe, 0);   // P

    } else {
        // se non sei in nessuno dei layer Fn, lascia tutto come è
    }
    return false;
}
