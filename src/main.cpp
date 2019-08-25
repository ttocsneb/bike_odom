#include "config.hpp"
#ifndef TEST
#include <Arduino.h>
#include "odom.hpp"
#include "led/display.hpp"


void setup() {
    odom::setup();
    display::setup();
}

const uint16_t Update_Time = 1000000 / (200*4); // 200Hz display rate

void loop() {
    display::update();
    delayMicroseconds(Update_Time);
}
#endif