#ifndef __CONFIG__
#define __CONFIG__

// Import all used headers here
#include "led/display.hpp"

#include <stdint.h>
// uncomment to enable debug output through serial
// #define DEBUG
#define TEST
// #define TEST_DISPLAY
#define TEST_TEXT

// The wheel sensor pin is required to be on an INT pin.  on atmega328p: the INT pins are on 2 and 3
const uint8_t WheelSensorPin = 2;

// These constants are used by the default implementations and used for interrupts
// They should be connected to PCINT pins.
// Comment out a button pin if it is not used
#define MODE_BUTTON_PIN 8
#define UNIT_BUTTON_PIN 9
#define RESET_BUTTON_PIN 10

// Pin Setup.  This is how the pins will be setup, again if you are seting up the pins
// yourself, or a pin is not in use, comment out the definition
#define MODE_BUTTON_PIN_SETUP pinMode(MODE_BUTTON_PIN, INPUT_PULLUP)
#define UNIT_BUTTON_PIN_SETUP pinMode(UNIT_BUTTON_PIN, INPUT_PULLUP)
#define RESET_BUTTON_PIN_SETUP pinMode(RESET_BUTTON_PIN, INPUT_PULLUP)

// When an interrupt is triggered.
#define MODE_BUTTON_MODE FALLING
#define UNIT_BUTTON_MODE FALLING
#define RESET_BUTTON_MODE FALLING

// Custom Functions
// Note: to use a function, you will need to include the header file that
// declares the function

/**
 * Button Functions (optional)
 * use a function to override the default logic for each button
 * It is called when a pin (not necessarily this pin) changes state
 * 
 * @return bool whether the button is pressed
 * 
 * function decleration example:
 * ```
 * bool modeButton() {}
 * #define MODE_BUTTON_FUNCTION modeButton
 * ```
 * 
 * The logic for each button is performed when the state changes from
 * false -> true, and doesn't repeat until 
 */
// #define MODE_BUTTON_FUNCTION // function
// #define UNIT_BUTTON_FUNCTION // function
// #define RESET_BUTTON_FUNCTION // function
// #define WHEEL_DETECTOR_FUNCTION // function

/**
 * Update Display (required)
 * 
 * updates the display.
 * 
 * there are 3 modes that the display can be in:
 * tachometer (rpm), speedometer (km/h / mph), odometer (km/mi)
 * 
 * There are 2 unit modes: metric, imperial.  The tachometer uses rpm
 * for both.
 * 
 * @param value float
 * @param mode rom::Mode
 * @param unit rom::Unit
 * 
 * function declaration example:
 * ```
 * void updateDisplay(float value, rom::Mode mode, rom::Unit unit)
 * #define UPDATE_DISPLAY_FUNCTION updateDisplay
 * ```
 */
#define UPDATE_DISPLAY_FUNCTION display::display

/**
 * Before sleep and After wake (optional)
 * 
 * Called just (before sleep / after wake).  This can be used to put other
 * devices to sleep/wake to save power
 * 
 * function declaration example:
 * ```
 * void beforeSleep() {}
 * void afterWake() {}
 * #define BEFORE_SLEEP beforeSleep
 * #define AFTER_WAKE afterWake
 * ```
 */
#define BEFORE_SLEEP display::onSleep
// #define AFTER_WAKE // function


// Debug macros
// you can use these macros, or add your own for debugging
#ifdef DEBUG
  #define PRINT(...) Serial.print(__VA_ARGS__)
  #define PRINTLN(...) Serial.println(__VA_ARGS__)
  #define DEBUG(cmd) cmd
#else
  #define PRINT(...) 
  #define PRINTLN(...)
  #define DEBUG(cmd)
#endif

#endif