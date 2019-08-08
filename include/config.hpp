#ifndef __CONFIG__
#define __CONFIG__

// Import all used headers here

#include <stdint.h>
// uncomment to enable debug output through serial
// #define DEBUG

const uint8_t WheelSensorPin = 5;
const uint8_t ModeButtonPin = 6;
const uint8_t UnitButtonPin = 7;
const uint8_t ResetButtonPin = 8;

// Custom Functions
// Note: to use a function, you will need to include the header file that
// declares the function

/**
 * Button Functions:
 * use a function to override the default logic for each button
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
 * Update Display
 * 
 * updates the display.
 * 
 * the format of the value will be a number with 1 decimal place.
 * Because the value is an integer, the value is increased by a factor
 * of 10 ie. 15.2 -> 152
 * 
 * there are 3 modes that the display can be in:
 * tachometer (rpm), speedometer (km/h / mph), odometer (km/mi)
 * 
 * There are 2 unit modes: metric, imperial.  The tachometer uses rpm
 * for both.
 * 
 * @param value unsigned int
 * @param mode rom::Mode
 * @param unit rom::Unit
 * 
 * function declaration example:
 * ```
 * void updateDisplay(unsigned int value, rom::Mode mode, rom::Unit unit)
 * #define UPDATE_DISPLAY_FUNCTION updateDisplay
 * ```
 */
#define UPDATE_DISPLAY_FUNCTION // function

/**
 * Before sleep and After wake
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
// #define BEFORE_SLEEP // function
// #define AFTER_WAKE // function

// Debug macros
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