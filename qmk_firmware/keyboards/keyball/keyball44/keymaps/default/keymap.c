/*
Copyright 2022 @Yowkees
Copyright 2022 MURAOKA Taro (aka KoRoN, @kaoriya)

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

#include QMK_KEYBOARD_H

#include "quantum.h"

// Custom keycodes for DPI and acceleration
enum custom_keycodes {
  DPI_800 = SAFE_RANGE,
  DPI_1200,
  DPI_INC, // DPI +100
  DPI_DEC, // DPI -100
  ACC_TOG, // Acceleration ON/OFF
};

// Global variable for acceleration mode
static bool acceleration_enabled = true;

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_universal(
    KC_TAB   , KC_Q     , KC_W     , KC_E     , KC_R     , KC_T     ,                                 KC_Y     , KC_U     , KC_I     , KC_O     , KC_P     , KC_MINS  ,
    KC_LCTL  , KC_A     , KC_S     , KC_D     , KC_F     , KC_G     ,                                 KC_H     , KC_J     , KC_K     , KC_L     , KC_SCLN  , KC_QUOT  ,
    KC_LSFT  , KC_Z     , KC_X     , KC_C     , KC_V     , KC_B     ,                                 KC_N     , KC_M     , KC_COMM  , KC_DOT   , KC_SLSH  , KC_BSPC  ,
                          KC_NO    , KC_LALT  , KC_LGUI  ,MO(1)     , KC_SPC   ,           KC_ENT   , MO(2)    , KC_NO    , KC_NO               , TG(3)
  ),

  [1] = LAYOUT_universal(
    KC_GRAVE , KC_1     , KC_2     , KC_3     , KC_4     , KC_5     ,                                 KC_6     , KC_7     , KC_8     , KC_9     , KC_0     , KC_EQL   ,
    _______  , KC_LNG1  , KC_NO    ,G(KC_PSCR), KC_HOME  , KC_PGUP  ,                                 KC_NO    , KC_NO    , KC_NO    , KC_LBRC  , KC_RBRC  , KC_BSLS  ,
    _______  , KC_LNG2  ,S(KC_CAPS), KC_NO    , KC_END   , KC_PGDN  ,                                 KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,
                          KC_NO    , _______  , _______  , _______  , _______  ,           _______  , KC_NO    , _______  , _______             , _______
  ),

  [2] = LAYOUT_universal(
    KC_ESC   , KC_F1    , KC_F2    , KC_F3    , KC_F4    , KC_F5    ,                                 KC_F6    , KC_F7    , KC_F8    , KC_F9    , KC_F10   , KC_F11   ,
    _______  , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                                 KC_LEFT  , KC_DOWN  , KC_UP    , KC_RGHT  , KC_NO    , KC_F12   ,
    _______  , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                                 KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_DEL   ,
                          KC_NO    , _______  , _______  , KC_NO    , _______  ,           _______  , _______  , _______  , _______             , _______
  ), 

  [3] = LAYOUT_universal(
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                                 KC_NO    , KC_NO    , KC_NO    , DPI_DEC  , DPI_INC  , ACC_TOG  ,
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                                 KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,
    KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,                                 KC_NO    , KC_NO    , MS_BTN1  , MS_BTN2  , MO(4)    , KC_NO    ,
                          QK_BOOT  , KC_NO    , KC_NO    , KC_NO    , KC_NO    ,           DPI_800  , DPI_1200 , _______  , _______             , TG(0)
  ),

  [4] = LAYOUT_universal(
    _______  , _______  , _______  , _______  , _______  , _______  ,                                 _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                 _______  , _______  , _______  , _______  , _______  , _______  ,
    _______  , _______  , _______  , _______  , _______  , _______  ,                                 _______  , _______  , _______  , _______  , _______  , _______  ,
                          _______  , _______  , _______  , _______  , _______  ,           _______  , _______  , _______  , _______             , _______
  ),
};
// clang-format on

layer_state_t layer_state_set_user(layer_state_t state) {
  // Enable scroll mode only when layer 4 is active
  keyball_set_scroll_mode(get_highest_layer(state) == 4);
  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode)
  {
  case DPI_800:
    if (record->event.pressed)
    {
      keyball_set_cpi(8); // 800 CPI
    }
    return false;
  case DPI_1200:
    if (record->event.pressed)
    {
      keyball_set_cpi(12); // 1200 CPI
    }
    return false;
  case DPI_INC:
    if (record->event.pressed)
    {
      uint8_t current_cpi = keyball_get_cpi();
      if (current_cpi < 50)
      {                                   // Max CPI is around 50 (5000 DPI)
        keyball_set_cpi(current_cpi + 1); // Increase by 100 DPI
      }
    }
    return false;
  case DPI_DEC:
    if (record->event.pressed)
    {
      uint8_t current_cpi = keyball_get_cpi();
      if (current_cpi > 1)
      {                                   // Min CPI is 1 (100 DPI)
        keyball_set_cpi(current_cpi - 1); // Decrease by 100 DPI
      }
    }
    return false;
  case ACC_TOG:
    if (record->event.pressed)
    {
      acceleration_enabled = !acceleration_enabled;
    }
    return false;
  }
  return true;
}

bool process_pointing_device_user(report_mouse_t *mouse_report) {
  // Disable trackball input on layers 0-2
  uint8_t highest_layer = get_highest_layer(layer_state | default_layer_state);
  if (highest_layer < 3)
  {
    // Clear all trackball movements and scrolling
    mouse_report->x = 0;
    mouse_report->y = 0;
    mouse_report->h = 0;
    mouse_report->v = 0;
    return true;
  }

  // Apply pointer acceleration on layers 3-4 when enabled
  if (acceleration_enabled && (highest_layer == 3 || highest_layer == 4)) {
    // Calculate speed squared
    int16_t speed_sq = mouse_report->x * mouse_report->x +
                       mouse_report->y * mouse_report->y;

    // Apply acceleration if speed exceeds threshold
    if (speed_sq > ACCELERATION_SPEED_THRESHOLD * ACCELERATION_SPEED_THRESHOLD)
    {
      float accel_multiplier = 1.0 + (ACCELERATION_STRENGTH * speed_sq / 1000.0);
      if (accel_multiplier > 3.0)
      {
        accel_multiplier = 3.0; // Limit max acceleration
      }

      mouse_report->x = (int8_t)(mouse_report->x * accel_multiplier);
      mouse_report->y = (int8_t)(mouse_report->y * accel_multiplier);
    }
  }

  return true;
}

#ifdef OLED_ENABLE

#include "lib/oledkit/oledkit.h"

void oledkit_render_info_user(void) {
  keyball_oled_render_keyinfo();
  keyball_oled_render_ballinfo();
  keyball_oled_render_layerinfo();
}
#endif
