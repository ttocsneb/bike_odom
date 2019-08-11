#ifndef __LED_DISPLAY__
#define __LED_DISPLAY__

#include "rom.hpp"

namespace display {
    void setup();
    void update();
    void display(float value, rom::Mode mode, rom::Unit unit);
    void onSleep();
}

#endif