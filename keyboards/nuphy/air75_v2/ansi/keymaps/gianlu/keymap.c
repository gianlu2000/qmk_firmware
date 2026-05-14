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
static int macro_origin_layer = -1; // layer in cui è stata avviata la macro
static bool macro_finished = false; // flag per indicare che la macro è finita e stiamo mostrando il segnale finale
static uint16_t macro_finish_timer = 0; // timer per la durata del segnale finale
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
static int get_layer_indicator_led_index(int layer_of_interest);

/**
 * process_record_user
 * Quando: chiamata su ogni evento tasto dal core QMK.
 * Scopo: intercettare i keycode custom per avviare macro; mentre una macro è in
 *        esecuzione blocca tutti i tasti eccetto `KC_ESC` (stop emergenza).
 * Parametri:
 *   - `keycode`: codice del tasto generato dall'evento.
 *   - `record`: puntatore a `keyrecord_t` con stato (pressed/released) e posizione del tasto.
 * Ritorno: `true` per permettere l'elaborazione standard del tasto, `false` per consumare l'evento.
 */
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
                macro_origin_layer = -1;
                macro_finished = false;
                macro_finish_timer = 0;
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
// Questa è un'implementazione molto semplice che invia i caratteri di una stringa uno alla volta ogni text_char_delay millisecondi, senza bloccare il loop principale di QMK.
/**
 * start_macro(const char *s, int led_index)
 * Quando: chiamata da `process_record_user` quando si preme un keycode di macro.
 * Scopo: inizializza lo stato della macro (stringa da inviare, timer, LED), imposta il
 *        driver RGB in modalità colore fisso e accende il LED di ESC e quello del tasto
 *        che ha avviato la macro.
 * Parametri:
 *   - `s`: puntatore alla stringa (testo) da inviare.
 *   - `led_index`: indice LED (g_led_config.matrix_co[row][col]) del tasto che ha avviato la macro;
 *                  se negativo viene ignorato.
 * Effetti collaterali: setta `macro_running=true`, `macro_origin_layer=biton32(layer_state)`.
 */
static void start_macro(const char *s, int led_index) {
    if (!s) return;
    macro_str = s;
    macro_len = (uint16_t)strlen(s);
    macro_index = 0;
    macro_timer = timer_read();
    macro_running = true;
    // registra il layer corrente come layer d'origine della macro
    macro_origin_layer = biton32(layer_state);

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

/**
 * send_char_nonblocking(char c)
 * Quando: chiamata internamente da `matrix_scan_user` per inviare singoli caratteri ASCII senza bloccare il loop.
 * Scopo: mappare caratteri comuni a `tap_code` appropriati; gestisce newline, spazio, punteggiatura,
 *        cifre e lettere (gestisce maiuscole usando il tasto Shift).
 * Parametri:
 *   - `c`: carattere ASCII da inviare.
 * Nota: i caratteri non supportati vengono ignorati (fallback).
 */
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

/**
 * send_unicode_nonblocking(uint16_t cp)
 * Quando: chiamata internamente da `matrix_scan_user` quando viene rilevata una sequenza UTF-8
 *        corrispondente a un code point supportato.
 * Scopo: inviare caratteri accentati italiani (e altri code point implementati) tramite sequenze
 *        di tasti e dead-key in modo non bloccante.
 * Parametri:
 *   - `cp`: code point Unicode (es. 0x00E0 per 'à').
 * Nota: implementa solo un sottoinsieme di code point comuni; gli altri vengono ignorati.
 */
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


/**
 * matrix_scan_user
 * Quando: chiamata ad ogni scan della matrice dal core QMK (loop principale).
 * Scopo: motore non-bloccante per l'invio delle macro; invia il prossimo carattere della
 *        stringa della macro ogni `text_char_delay` millisecondi, gestisce sequenze UTF-8
 *        e segnala il completamento della macro (imposta `macro_finished`).
 * Parametri: nessuno.
 * Effetti collaterali: quando la macro termina setta `macro_finished=true`, avvia il
 *        `macro_finish_timer` e spegne immediatamente il LED di stop (ESC) per preparare
 *        il segnale finale di completamento.
 */
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
            // la macro è terminata: entriamo nello stato "finished"
            // Manteniamo macro_led_index e macro_origin_layer per il segnale finale
            macro_running = false;
            macro_str = NULL;
            macro_len = 0;
            macro_index = 0;
            macro_finish_timer = timer_read();
            macro_finished = true;
            // spegni immediatamente il LED di stop (ESC)
            int esc_led_index = g_led_config.matrix_co[0][0];
            if (esc_led_index != NO_LED && esc_led_index >= 0 && esc_led_index < RGB_MATRIX_LED_COUNT) {
                rgb_matrix_set_color((uint8_t)esc_led_index, 0, 0, 0);
            }
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

// layer 4 - created by NuPhy, lasciarlo pulito
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


/**
 * layer_state_set_user
 * Quando: chiamata dal core QMK quando cambia lo stato dei layer.
 * Scopo: quando si entra nei layer 5 o 6 (layer macro) mette in pausa l'effetto RGB e spegne i LED;
 *        quando si esce ripristina l'effetto salvato dall'EEPROM.
 * Parametri:
 *   - `state`: nuovo `layer_state_t` che rappresenta i layer attivi.
 * Ritorno: deve restituire il `layer_state_t` risultante (qui viene semplicemente restituito `state`).
 */
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

/**
 * get_led_color_for_index(uint8_t idx, uint8_t *r, uint8_t *g, uint8_t *b)
 * Quando: invocata dal codice di indicazione RGB per ottenere il colore di un LED specifico.
 * Scopo: impostare `r,g,b` in base al "layer di interesse". Se una macro è in esecuzione
 *        usa `macro_origin_layer` come riferimento, altrimenti usa il layer corrente.
 * Parametri:
 *   - `idx`: indice del LED (0..RGB_MATRIX_LED_COUNT-1).
 *   - `r,g,b`: puntatori ai valori R,G,B che verranno impostati dalla funzione.
 * Output: imposta i valori attraverso i puntatori; fallback arancione se non ci sono corrispondenze.
 */
static void get_led_color_for_index(uint8_t idx, uint8_t *r, uint8_t *g, uint8_t *b) {
    // fallback: arancione
    *r = 255; *g = 160; *b = 0;

    // Se c'è una macro in esecuzione useremo come riferimento il layer
    // in cui la macro è stata avviata (macro_origin_layer). Altrimenti
    // usiamo il layer corrente.
    int layer_of_interest = -1;
    if (macro_running && macro_origin_layer >= 0) {
        layer_of_interest = macro_origin_layer;
    } else {
        layer_of_interest = biton32(layer_state);
    }

    if (layer_of_interest == 5) {
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
    } else if (layer_of_interest == 6) {
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

// Ritorna l'indice LED usato come "indicatore di layer" per i layer di macro
static int get_layer_indicator_led_index(int layer_of_interest) {
    if (layer_of_interest == 5) {
        return 40; // tasto O (layer Outlook)
    }
    if (layer_of_interest == 6) {
        return 41; // tasto P (layer Ticket)
    }
    return -1;
}

// Ritorna il colore statico (no-breathe) da usare quando il tasto layer deve solo lampeggiare
static void get_static_layer_color(int layer_of_interest, uint8_t *r, uint8_t *g, uint8_t *b) {
    // fallback: arancione (stesso colore della macro)
    *r = 255; *g = 160; *b = 0;
    if (layer_of_interest == 5) {
        *r = 0; *g = 0; *b = 255; // Outlook - blu statico
        return;
    }
    if (layer_of_interest == 6) {
        *r = 0; *g = 204; *b = 0; // Ticket - verde statico
        return;
    }
}

/**
 * rgb_matrix_indicators_user
 * Quando: chiamata dal driver RGB per applicare indicatori custom ad ogni ciclo di aggiornamento RGB.
 * Scopo: mostrare lo stato delle macro o i colori dei layer:
 *   - se `macro_running`: spegne tutti i LED, accende ESC (rosso) e fa lampeggiare il LED della macro;
 *   - se `macro_finished`: mostra il LED della macro in verde per 1s;
 *   - altrimenti: mostra i colori specifici dei layer (5 e 6) o lascia gli effetti normali.
 * Parametri: nessuno.
 * Ritorno: `false` per indicare che la gestione custom è stata applicata.
 */
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

        // indice LED del tasto indicatore del layer di origine (es. O per layer 5, P per layer 6)
        int layer_indicator = get_layer_indicator_led_index(macro_origin_layer);

        // Calcola i colori per il tasto layer (se presente) usando colore statico (no-breathe)
        uint8_t lr = 0, lg = 0, lb = 0;
        if (layer_indicator >= 0 && layer_indicator < RGB_MATRIX_LED_COUNT) {
            get_static_layer_color(macro_origin_layer, &lr, &lg, &lb);
        }

        // Lampeggio: macro key in arancione fisso, tasto layer nel suo colore assegnato
        uint16_t t = timer_read();
        bool on = (((t / 300) & 1) == 0); // 300ms on/off
        if (on) {
            if (macro_led_index >= 0 && macro_led_index < RGB_MATRIX_LED_COUNT) {
                rgb_matrix_set_color((uint8_t)macro_led_index, 255, 160, 0); // arancione
            }
            if (layer_indicator >= 0 && layer_indicator < RGB_MATRIX_LED_COUNT) {
                rgb_matrix_set_color((uint8_t)layer_indicator, lr, lg, lb);
            }
        } else {
            // off: lascia spenti (già resettati sopra)
        }
        return false;
    }
    if (macro_finished) {
        // durante il periodo di segnale finale mostra il LED della macro in verde (1s)
        if (timer_elapsed(macro_finish_timer) < 1000) {
            for (uint16_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
                rgb_matrix_set_color(i, 0, 0, 0);
            }
            int esc_led = g_led_config.matrix_co[0][0];
            if (esc_led != NO_LED && esc_led >= 0 && esc_led < RGB_MATRIX_LED_COUNT) {
                rgb_matrix_set_color((uint8_t)esc_led, 0, 0, 0);
            }
            if (macro_led_index >= 0 && macro_led_index < RGB_MATRIX_LED_COUNT) {
                rgb_matrix_set_color((uint8_t)macro_led_index, 0, 204, 0);
            }
            // mostra anche il tasto indicatore del layer in verde per 1s (coordinato col tasto macro) - temporaneamente disattivato
            /* int layer_indicator = get_layer_indicator_led_index(macro_origin_layer);
            if (layer_indicator >= 0 && layer_indicator < RGB_MATRIX_LED_COUNT) {
                rgb_matrix_set_color((uint8_t)layer_indicator, 0, 204, 0);
            } */
            return false;
        } else {
            // termina il periodo di segnale e ripristina l'effetto precedente
            macro_finished = false;
            macro_led_index = -1;
            macro_origin_layer = -1;
            rgb_matrix_reload_from_eeprom();
        }
    }
    if (layer_state_is(5)) {
        // se una macro è in riproduzione e l'origine è questo layer, non applicare l'effetto respiro
        if (macro_running && macro_origin_layer == 5) {
            return false;
        }
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
        // se una macro è in riproduzione e l'origine è questo layer, non applicare l'effetto respiro
        if (macro_running && macro_origin_layer == 6) {
            return false;
        }
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
