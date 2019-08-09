#include "hardware.hpp"
#include "odom.hpp"
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <PinChangeInterrupt.h>

void onModePress() {
    bool pressed = MODE_BUTTON_FUNCTION();
}
void onUnitPress() {
    bool pressed = UNIT_BUTTON_FUNCTION();
}
void onResetPress() {
    bool pressed = RESET_BUTTON_FUNCTION();
}

void onWheelRotate() {
    if(WHEEL_DETECTOR_FUNCTION()) {
        odom::time_blocks[odom::active_block]++;
        odom::sleep_timer = 0;
    }
}

void hardware::setup() {
    #ifdef MODE_BUTTON_PIN_SETUP
      MODE_BUTTON_PIN_SETUP;
    #endif
    #ifdef UNIT_BUTTON_PIN_SETUP
      UNIT_BUTTON_PIN_SETUP;
    #endif
    #ifdef RESET_BUTTON_PIN_SETUP
      RESET_BUTTON_PIN_SETUP;
    #endif
    
    #ifdef MODE_BUTTON_PIN
      attachPCINT(digitalPinToPCINT(MODE_BUTTON_PIN), onModePress, MODE_BUTTON_MODE);
    #endif
    #ifdef UNIT_BUTTON_PIN
      attachPCINT(digitalPinToPCINT(UNIT_BUTTON_PIN), onModePress, UNIT_BUTTON_MODE);
    #endif
    #ifdef RESET_BUTTON_PIN
      attachPCINT(digitalPinToPCINT(RESET_BUTTON_PIN), onModePress, RESET_BUTTON_MODE);
    #endif

    attachInterrupt(digitalPinToInterrupt(WheelSensorPin), onWheelRotate, FALLING);
}

void wake() {
    sleep_disable();
    detachInterrupt(digitalPinToInterrupt(WheelSensorPin));
    attachInterrupt(digitalPinToInterrupt(WheelSensorPin), onWheelRotate, FALLING);
    PRINTLN("Just Woke up");
    #ifdef AFTER_WAKE
      AFTER_WAKE();
    #endif
}

void hardware::sleep() {
    PRINTLN("Going to sleep");
    #ifdef BEFORE_SLEEP
      BEFORE_SLEEP();
    #endif
    sleep_enable();
    // Set to wake when the Wheel sensor pin goes low
    detachInterrupt(digitalPinToInterrupt(WheelSensorPin));
    attachInterrupt(digitalPinToInterrupt(WheelSensorPin), wake, FALLING);
    sleep_cpu();
}

const uint16_t PressTime = 200;

bool hardware::modeButton() {
    static uint32_t lastPress(0);
    uint32_t time = millis();
    bool ret = time - lastPress > PressTime;
    lastPress = time;
    return ret;
}

bool hardware::unitButton() {
    static uint32_t lastPress(0);
    uint32_t time = millis();
    bool ret = time - lastPress > PressTime;
    lastPress = time;
    return ret;
}

bool hardware::resetButton() {
    static uint32_t lastPress(0);
    uint32_t time = millis();
    bool ret = time - lastPress > PressTime;
    lastPress = time;
    return ret;
}

bool hardware::wheelDetector() {
    static uint32_t lastPress(0);
    uint32_t time = millis();
    bool ret = time - lastPress > 5;
    lastPress = time;
    return ret;
}