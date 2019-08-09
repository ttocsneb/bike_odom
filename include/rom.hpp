#ifndef __ROM__
#define __ROM__

#include <stdint.h>

/**
 * Contains all variables that will persist
 */
namespace rom {
    extern const uint8_t Block_Time;
    extern const uint8_t Average_Time;
    extern const uint8_t Num_Magnets;
    extern const uint8_t Diameter;
    extern const uint8_t Sleep_Time;
    extern const uint8_t Distance;
    extern const uint8_t DistanceUpdateTime;

    extern const uint8_t Bit_Mode;
    extern const uint8_t Bit_Unit;

    uint8_t read_byte(uint16_t address);
    uint16_t read_int(int16_t address);
    uint32_t read_long(uint16_t address);
    uint8_t read_bits(uint8_t bit);


    void write(uint16_t address, uint32_t val);
    void write_bits(uint8_t bit, uint8_t val);

    enum Mode {TACHO, SPEED, ODOM};
    enum Unit {METRIC, IMPERIAL};
}

#endif