#include "config.hpp"
#ifdef TEST_TEXT
#include <Arduino.h>
#include <led/display.hpp>
#include <rom.hpp>

void setup() {
    Serial.begin(115200);
    display::setup();
    display::display(0, rom::ODOM, rom::METRIC);
}


const uint16_t Update_Time = 1000000 / (200*4); // 200Hz display rate


void loop() {
    static uint8_t counter(0);

    if (counter > 50) {
        counter = 0;
        display::display(millis() / 1000.0, rom::ODOM, rom::METRIC);
    }

    ++counter;
    delayMicroseconds(Update_Time);
}


#endif