/*
 * ESPButton.h
 * Ticker-scan based Button Handler with debounce.
 * Default scan interval is 16ms (60FPS).
 * All added Buttons are scanned by a global Ticker (by os timer).
 *
 * Usage:
 * in setup():
 * pinMode(pin, INPUT_PULLUP / INPUT /... );
 * ESPButton.add(id, pin, pin_down_digital);
 * ESPButton.setCallback(...); // handle your ButtonEvent by id
 * ESPButton.begin(); // call begin to start scan.
 * in loop():
 * ESPButton.loop(); // will notify the event in loop (not in interrupt timer)
 *
 *  Created on:  2020-03-08
 *  Last update: 2020-11-03
 *      Author:  Wang Bin/Caffreyfans
 * 
 */

#ifndef ESPBUTTON_H_
#define ESPBUTTON_H_

#include <Arduino.h>
#include <functional>
#include <vector>

#define ESPBUTTON_DEBUG(message, ...)  //printf_P(PSTR("[%7d] ESPButton: " message "\n"), millis(), ##__VA_ARGS__)


void IRAM_ATTR buttonISR();

typedef enum {
	ESPBUTTONEVENT_NONE = 0,
	ESPBUTTONEVENT_SINGLECLICK,
	ESPBUTTONEVENT_DOUBLECLICK,
	ESPBUTTONEVENT_LONGCLICK
} ESPButtonEvent;

typedef std::function<void(int pin, ESPButtonEvent event)> ESPButtonCallback;

typedef struct _ESPButtonEntry {
	uint8_t id = -1;
	uint8_t pin = -1;
	uint8_t pin_down_digital = LOW; // 按下状态时的digital值
	uint8_t pin_up_digital = HIGH; // 按下状态时的digital值
	uint16_t long_press_interval = 500;
	uint16_t single_click_interval = 50;
	ESPButtonEvent event;
	ESPButtonCallback callback;
} ESPButtonEntry;

extern std::vector<ESPButtonEntry> v_entry;

class ESPButtonClass {
public:
	ESPButtonClass() {}

	void add(uint8_t pin, ESPButtonCallback callback) {
		ESPButtonEntry entry;
		entry.pin = pin;
		entry.callback = callback;
		attachInterrupt(entry.pin, buttonISR, ONLOW);
		v_entry.push_back(entry);
	}

	PGM_P getButtonEventDescription(ESPButtonEvent e);

	void loop();
};

extern ESPButtonClass ESPButton;
#endif /* ESPBUTTON_H_ */
