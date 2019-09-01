#include "hardware.hpp"
#include "odom.hpp"
#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <PinChangeInterrupt.h>

#define INT_WHEEL_ROTATE attachInterrupt(digitalPinToInterrupt(WHEEL_SENSOR_PIN), &onWheelRotate, RISING)
#define INT_WHEEL_WAKE attachInterrupt(digitalPinToInterrupt(WHEEL_SENSOR_PIN), wake, RISING)

void onModePress() {
    if (MODE_BUTTON_FUNCTION()) {
      odom::nextMode();
    }
}
void onUnitPress() {
    if (UNIT_BUTTON_FUNCTION()) {
      odom::nextUnit();
    }
}
void onResetPress() {
    if (RESET_BUTTON_FUNCTION()) {
      odom::resetOdom();
    }
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
    #ifdef WHEEL_SENSOR_SETUP
      WHEEL_SENSOR_SETUP;
    #endif
    
    #ifdef MODE_BUTTON_PIN
      attachPCINT(digitalPinToPCINT(MODE_BUTTON_PIN), onModePress, MODE_BUTTON_MODE);
    #endif
    #ifdef UNIT_BUTTON_PIN
      attachPCINT(digitalPinToPCINT(UNIT_BUTTON_PIN), onUnitPress, UNIT_BUTTON_MODE);
    #endif
    #ifdef RESET_BUTTON_PIN
      attachPCINT(digitalPinToPCINT(RESET_BUTTON_PIN), onResetPress, RESET_BUTTON_MODE);
    #endif

    INT_WHEEL_ROTATE;
}

void wake() {
    sleep_disable();
    detachInterrupt(digitalPinToInterrupt(WHEEL_SENSOR_PIN));
    INT_WHEEL_ROTATE;
    odom::setup();
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
    detachInterrupt(digitalPinToInterrupt(WHEEL_SENSOR_PIN));
    INT_WHEEL_WAKE;
    sleep_cpu();
}

const uint16_t PressTime = 250;

bool isPressed(uint32_t &lastPress, uint16_t pressTime) {
  uint32_t time = millis();
  bool ret = time - lastPress > pressTime;
  if (ret) {
    lastPress = time;
  }
  return ret;
}

bool hardware::modeButton() {
    static uint32_t lastPress(0);
    return isPressed(lastPress, PressTime);
}

bool hardware::unitButton() {
    static uint32_t lastPress(0);
    return isPressed(lastPress, PressTime);
}

bool hardware::resetButton() {
    static uint32_t lastPress(0);
    return isPressed(lastPress, PressTime);
}

bool hardware::wheelDetector() {
    static uint32_t lastPress(0);
    return isPressed(lastPress, 100);
}