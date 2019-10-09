#include "hardware.hpp"
#include "odom.hpp"
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <PinChangeInterrupt.h>

#define INT_WHEEL(x) attachInterrupt(digitalPinToInterrupt(WHEEL_SENSOR_PIN), x, WHEEL_TRIGGER_MODE)

void onWheelRotate() {
    if(WHEEL_DETECTOR_FUNCTION()) {
        odom::time_blocks[odom::active_block]++;
        odom::sleep_timer = 0;
    }
}

void hardware::setup() {
    #ifdef WHEEL_SENSOR_SETUP
      WHEEL_SENSOR_SETUP;
    #endif

    INT_WHEEL(&onWheelRotate);
}

void wake() {
    sleep_disable();
    detachInterrupt(digitalPinToInterrupt(WHEEL_SENSOR_PIN));
    INT_WHEEL(&onWheelRotate);
    odom::setup();
    PRINTLN("Just Woke up");
    #ifdef AFTER_WAKE
      AFTER_WAKE();
    #endif
}

void hardware::sleep() {
    noInterrupts();
    PRINTLN("Going to sleep");
    #ifdef BEFORE_SLEEP
      BEFORE_SLEEP();
    #endif
    // Set to wake when the Wheel sensor pin goes low
    detachInterrupt(digitalPinToInterrupt(WHEEL_SENSOR_PIN));

    // Start sleeping
    sleep_enable();
    INT_WHEEL(&wake);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    interrupts();
    sleep_cpu();
}

bool hardware::wheelDetector() {
    static uint32_t lastPress(0);
    uint32_t time = millis();
    bool ret = time - lastPress > 10;
    if (ret) {
      lastPress = time;
    }
    return ret;
}