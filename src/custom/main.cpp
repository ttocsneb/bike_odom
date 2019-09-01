#include "config.hpp"
#ifndef TEST
#include <Arduino.h>
#include "odom.hpp"
#include "custom/display.hpp"


void setup() {
    odom::setup();
    display::setup();
}

void loop() {

}
#endif