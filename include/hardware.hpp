#ifndef __HARDWARE__
#define __HARDWARE__

#include "config.hpp"

namespace hardware {
    bool modeButton();
    bool unitButton();
    bool resetButton();
    bool wheelDetector();

    void setup();
    void sleep();
}

#ifndef MODE_BUTTON_FUNCTION
  #define MODE_BUTTON_FUNCTION hardware::modeButton
#endif
#ifndef UNIT_BUTTON_FUNCTION
  #define UNIT_BUTTON_FUNCTION hardware::unitButton
#endif
#ifndef RESET_BUTTON_FUNCTION
  #define RESET_BUTTON_FUNCTION hardware::resetButton
#endif
#ifndef WHEEL_DETECTOR_FUNCTION
  #define WHEEL_DETECTOR_FUNCTION hardware::wheelDetector
#endif

#endif