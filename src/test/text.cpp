#include "config.hpp"
#ifdef TEST_TEXT
#include <Arduino.h>
#include <custom/display.hpp>
#include <rom.hpp>

void setup() {
    Serial.begin(115200);
    display::setup();
    display::display(0, rom::ODOM, rom::METRIC);
}

void loop() {

    display::display(millis() / 1000.0, rom::SPEED, rom::METRIC);
    delay(50);
}


#endif