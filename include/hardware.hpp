#ifndef __HARDWARE__
#define __HARDWARE__

#include "config.hpp"

namespace hardware {

    bool wheelDetector();

    void setup();
    void sleep();
}

#ifndef WHEEL_DETECTOR_FUNCTION
  #define WHEEL_DETECTOR_FUNCTION hardware::wheelDetector
#endif

#endif