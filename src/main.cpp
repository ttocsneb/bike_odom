#include <Arduino.h>
#include "rom.hpp"
#include "config.hpp"
#include "hardware.hpp"

uint32_t distance;
uint16_t average_time;

uint8_t num_blocks;
uint8_t* time_blocks = NULL;

uint8_t num_magnets;
float wheel_conversion;

uint32_t sleep_time;
uint8_t active_block;

rom::Mode mode;
rom::Unit unit;

const float mile = 1609.344;
const float kilometer = 1000;

void setup() {
  distance = rom::read_long(rom::Distance);
  average_time = rom::read_int(rom::Average_Time);
  num_magnets = rom::read_byte(rom::Num_Magnets);

  // Load the time blocks array
  num_blocks = average_time / rom::read_int(rom::Block_Time);
  if (time_blocks != NULL) {
    delete[] time_blocks;
  }
  time_blocks = new uint8_t[num_blocks];
  for (uint8_t i = 0; i < num_blocks; ++i) {
    time_blocks[i] = 0;
  }

  wheel_conversion = (rom::read_int(rom::Diameter) / 1000.0) * PI / num_magnets;

  sleep_time = rom::read_long(rom::Sleep_Time);

  active_block = 0;

  mode = rom::Mode(rom::read_bits(rom::Bit_Mode));
  unit = rom::Unit(rom::read_bits(rom::Bit_Unit));
}

void loop() {
  // put your main code here, to run repeatedly:
}

uint16_t sum_blocks() {
  uint16_t total = 0;
  for (uint8_t i = 0; i < num_blocks; ++i) {
    total += time_blocks[i];
  }
  return total;
}

float getUnitConversion() {
  return unit == rom::METRIC ? kilometer : mile;
}

void timer() {
  distance += time_blocks[active_block];

  float display;
  // Calculate the display value
  if (mode == rom::ODOM) {
    display = distance * wheel_conversion / getUnitConversion();
  } else {
    float tacho = sum_blocks() / (average_time / 1000.0);
    if (mode == rom::TACHO) {
      display = tacho / num_magnets;
    } else {
      display = tacho * wheel_conversion / getUnitConversion() * 3600;
    }
  }

  active_block += 1;
  if (active_block >= num_blocks) {
    active_block = 0;
  }
  time_blocks[active_block] = 0;

  UPDATE_DISPLAY_FUNCTION(static_cast<unsigned int>(display * 10), mode, unit);
}