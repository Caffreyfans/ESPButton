#include <Arduino.h>
#include "ESPButtonPlus.h"

#define PIN_BUTTON 0 // Use the Flash-Button of NodeMCU
#define LONG_PRESS_TIME 100

void button_callback(int pin, ESPButtonEvent event) {
  Serial.printf("%d %s\n", pin, ESPButton.getButtonEventDescription(event));
}

void setup()
{
  Serial.begin(115200);
  ESPButton.add(0, button_callback);
}

void loop()
{
  ESPButton.loop();
}