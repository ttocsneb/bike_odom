#include <Arduino.h>
#include "odom.hpp"
#include "led/display.hpp"

void setup() {
    odom::setup();
    display::setup();
}

const uint16_t Update_Time = 1000000 / (200*4);

void loop() {
    display::update();
    delayMicroseconds(Update_Time);
}