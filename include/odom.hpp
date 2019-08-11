#ifndef __MAIN__
#define __MAIN__

#include <stdint.h>

namespace odom {
    extern volatile uint8_t* time_blocks;
    extern volatile uint8_t active_block;
    extern volatile uint32_t sleep_timer;

    void setup();
}
#endif