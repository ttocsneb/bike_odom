#ifndef __LED_DISPLAY__
#define __LED_DISPLAY__

#include "rom.hpp"

namespace display {
    void setup();
    void update();
    void display(unsigned int value, rom::Mode mode, rom::Unit unit);
}

#endif