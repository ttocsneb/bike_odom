#ifndef __MAIN__
#define __MAIN__

#include <stdint.h>
#include "rom.hpp"

namespace odom {

    void resetTrip();

    extern volatile uint8_t* time_blocks;
    extern volatile uint8_t active_block;
    extern volatile uint32_t sleep_timer;

    void setup();
    void loop();
}
#endif