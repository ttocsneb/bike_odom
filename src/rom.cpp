#include "rom.hpp"
#include <EEPROM.h>


// These addresses aren't real eeprom addresses
// Each value is 4 bytes long, so the eeprom address is found by multiplying by 4
const uint8_t rom::Block_Time = 0x00;
const uint8_t rom::Average_Time = 0x01;
const uint8_t rom::Num_Magnets = 0x02;
const uint8_t rom::Diameter = 0x03;
const uint8_t rom::Sleep_Time = 0x04;
const uint8_t Bits = 0x05;
const uint8_t rom::Distance = 0x06;
const uint8_t rom::DistanceUpdateTime = 0x07;
const uint8_t New_Rom_Checker = 0x08;

const uint8_t rom::Bit_Mode = 0;
const uint8_t rom::Bit_Unit = 1;
const uint8_t BitLocation[2] = {0, 2};
const uint8_t BitSize[2] = {2, 1};

const uint32_t New_Rom_Default = 0x5a5a6b6b; // seemingly random number
const uint16_t Block_Time_Default = 250;
const uint16_t Average_Time_Default = 2000;
const uint8_t Num_Magnets_Default = 1;
const uint16_t Diameter_Default = 660;
const uint32_t Sleep_Time_Default = 300000;
const uint32_t Distance_Default = 0;
const uint32_t Distance_Update_Time_Default = 300000;
const uint8_t Bit_Mode_Default = 0;
const uint8_t Bit_Unit_Default = 0;

void rom::setup() {
    EEPROM.begin();

    // Set the default values if the rom hasn't been set yet.
    if (read_long(New_Rom_Checker) != New_Rom_Default) {
        write(Block_Time, Block_Time_Default);
        write(Average_Time, Average_Time_Default);
        write(Num_Magnets, Num_Magnets_Default);
        write(Diameter, Diameter_Default);
        write(Sleep_Time, Sleep_Time_Default);
        write(Distance, Distance_Default);
        write(DistanceUpdateTime, Distance_Update_Time_Default);
        write_bits(Bit_Mode, Bit_Mode_Default);
        write_bits(Bit_Unit, Bit_Unit_Default);

        write(New_Rom_Checker, New_Rom_Default);
    }
}

uint32_t read(uint16_t address) {
    return EEPROM.read(address);
}

uint8_t rom::read_byte(uint16_t address) {
    address *= 4;
    return read(address + 3);
}

uint16_t rom::read_int(int16_t address) {
    address *= 4;
    return (read(address + 2) << 8) | read(address + 3);
}

uint32_t rom::read_long(uint16_t address) {
    address *= 4;
    return (read(address) << 24)
        | (read(address + 1) << 16)
        | (read(address + 2) << 8)
        | read(address + 3);
}

uint8_t rom::read_bits(uint8_t bit) {
    return read_long(Bits) >> BitLocation[bit] // bit shift
        & (0xffffffff >> (32 - BitSize[bit])); // bit mask
}

void rom::write(uint16_t address, uint32_t val) {
    address *= 4;
    EEPROM.update(address, val >> 24);
    EEPROM.update(address + 1, val >> 16);
    EEPROM.update(address + 2, val >> 8);
    EEPROM.update(address + 3, val);
}

void rom::write_bits(uint8_t bit, uint8_t val) {
    uint32_t bits = read_long(Bits);
    // Clear the bits we will write to
    bits &= ~((0xffffffff >> (32 - BitSize[bit])) << BitLocation[bit]);
    bits |= (val & (0xffffffff >> (32 - BitSize[bit]))) << BitLocation[bit];
    write(Bits, bits);
}
