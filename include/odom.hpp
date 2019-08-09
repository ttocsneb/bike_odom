#ifndef __MAIN__
#define __MAIN__

#include <stdint.h>
#include <avr/interrupt.h>

// Calculate the scale and number of steps for the block timer based on the clock speed
#define F_CPU_M F_CPU / 1000
#define TIMER_SCALE (F_CPU_M < 255 ? 1 : (F_CPU_M / 8 < 255 ? 2 : (F_CPU_M / 64 < 255 ? 3 : (F_CPU_M / 256 ? 4 : 5))))
#define timer_to_scale(x) (x == 1 ? 1 : (x == 2 ? 8 : (x == 3 ? 64 : (x == 4 ? 256 : (x == 5 ? 1024 : 0)))))
#define TIMER_STOP (F_CPU_M / timer_to_scale(TIMER_SCALE))

namespace odom {
    extern volatile uint8_t* time_blocks;
    extern volatile uint8_t active_block;
    extern volatile uint32_t sleep_timer;

    void setup();
}
#endif