#ifndef __LED_DISPLAY__
#define __LED_DISPLAY__

#include "rom.hpp"

#define MODE_BUTTON_PIN 8
#define UNIT_BUTTON_PIN 9
#define RESET_BUTTON_PIN 10

#define MODE_BUTTON_MODE FALLING
#define UNIT_BUTTON_MODE FALLING
#define RESET_BUTTON_MODE CHANGE

#define pA 4
#define pB 5
#define pC 6
#define pD 7
#define pDP A5

#define pD0 A0
#define pD1 A1
#define pD2 A2
#define pD3 A3

#define pLATCH A4


namespace display {
    void setup();
    /**
     * Sets the text to be displayed on the display.
     * 
     * Only allows numbers and periods
     * 
     * '1.23.4'
     */
    void setText(char* text);
    void display(float rpm, float speed, float odom, float trip);
    void onSleep();
}

#endif