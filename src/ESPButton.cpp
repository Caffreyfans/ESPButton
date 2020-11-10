#include "ESPButtonPlus.h"
#include <math.h>

void IRAM_ATTR buttonISR() {
  static unsigned long last_on_low = millis();
  unsigned long on_low = millis();
  static volatile unsigned long last_key_down = millis();
  static volatile unsigned long last_key_up = millis();
  static unsigned long key_down = millis();
  static unsigned long key_up = millis();
  unsigned long interval = on_low - last_on_low;
  if (interval > 5) {  // key down
    for (int i = 0; i < v_entry.size(); i++) {
      if (digitalRead(v_entry[i].pin) == v_entry[i].pin_down_digital) {
        last_key_down = key_down;
        key_down = millis();
      }
    }
  }
  for (int i = 0; i < v_entry.size(); i++) {
    if (digitalRead(v_entry[i].pin) == v_entry[i].pin_up_digital) {  // maybe key up
      last_key_up = key_up;
      key_up = millis();
      if (key_up - last_key_up > 10) { // key up
        unsigned long key_down_keep = key_up - key_down;
        // ESPBUTTON_DEBUG("last key down = %lu\n", last_key_down);
        // ESPBUTTON_DEBUG("last key up = %lu\n", last_key_up);
        // ESPBUTTON_DEBUG("key down = %lu\n", key_down);
        // ESPBUTTON_DEBUG("key up = %lu\n\n", key_up);
        if (key_down_keep > v_entry[i].long_press_interval) {
          v_entry[i].event = ESPBUTTONEVENT_LONGCLICK;
        } else if (key_down_keep > v_entry[i].single_click_interval) {
          v_entry[i].event = ESPBUTTONEVENT_SINGLECLICK;
        }
      }
    }
  }
  last_on_low = on_low;
}

PGM_P ESPButtonClass::getButtonEventDescription(ESPButtonEvent e)
{
  switch (e) {
  case ESPBUTTONEVENT_SINGLECLICK:
    return PSTR("SingleClick");
  case ESPBUTTONEVENT_DOUBLECLICK:
    return PSTR("DoubleClick");
  case ESPBUTTONEVENT_LONGCLICK:
    return PSTR("LongClick");
  default:
    return PSTR("<unknown event>");
  }
}

void ESPButtonClass::loop()
{
  for (int i = 0; i < v_entry.size(); i++) {
    if (v_entry[i].event != ESPBUTTONEVENT_NONE) {
      v_entry[i].callback(v_entry[i].pin, v_entry[i].event);
      v_entry[i].event = ESPBUTTONEVENT_NONE;
    }
  }
}

std::vector<ESPButtonEntry> v_entry;
ESPButtonClass ESPButton;