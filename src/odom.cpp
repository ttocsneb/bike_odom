#include <Arduino.h>
#include "odom.hpp"
#include "rom.hpp"
#include "config.hpp"
#include "hardware.hpp"
#include "serial.hpp"

// Calculate the scale and number of steps for the block timer based on the clock speed
#define F_CPU_M F_CPU / 1000
#define MAX 65535
#define TIMER_SCALE 4
#define timer_to_scale(x) (x == 1 ? 1 : (x == 2 ? 8 : (x == 3 ? 64 : (x == 4 ? 256 : (x == 5 ? 1024 : 0)))))
const uint16_t TIMER_STEP_MS = (F_CPU_M / timer_to_scale(TIMER_SCALE));

volatile uint32_t distance;
volatile uint32_t trip_distance;
volatile uint16_t block_time;
volatile uint16_t average_time;

uint8_t num_blocks;
volatile uint8_t* odom::time_blocks = NULL;
volatile uint8_t odom::active_block;
volatile uint32_t odom::sleep_timer = 0;

uint8_t num_magnets;
float wheel_conversion;

uint32_t sleep_time;
uint32_t distance_update_time;

const float mile = 1609.344;
const float kilometer = 1000;

void timer();

void setupTimer() {
    // Setup timer 1 for the block timer

    // Clear the current settings
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;

    // Set the timer scale
    TCCR1B |= TIMER_SCALE;

    // Set the timer mode to mode 4: CTC (OCR1)
    TCCR1B |= (1 << WGM12);

    // Set the length of the timer (1 ms * block_time)
    OCR1A = TIMER_STEP_MS * block_time;

    TIMSK1 |= (1 << OCIE2A);
}

void odom::setup() {
    noInterrupts();
    rom::setup();

    distance = rom::read_long(rom::Distance);
    average_time = rom::read_int(rom::Average_Time);
    num_magnets = rom::read_byte(rom::Num_Magnets);
    block_time = rom::read_int(rom::Block_Time);
    distance_update_time = rom::read_long(rom::DistanceUpdateTime);

    // Load the time blocks array
    num_blocks = average_time / block_time;
    if (time_blocks != NULL) {
        delete[] time_blocks;
    }
    time_blocks = new uint8_t[num_blocks];
    for (uint8_t i = 0; i < num_blocks; ++i) {
        time_blocks[i] = 0;
    }

    wheel_conversion = (rom::read_int(rom::Diameter) / 1000.0) * PI;

    sleep_time = rom::read_long(rom::Sleep_Time);

    active_block = 0;


    serial::setup();
    hardware::setup();
    setupTimer();
    interrupts();
}

void odom::resetTrip() {
    trip_distance = 0;
    rom::write(rom::Trip, 0);
}

uint16_t sum_blocks() {
    uint16_t total = 0;
    for (uint8_t i = 0; i < num_blocks; ++i) {
        total += odom::time_blocks[i];
    }
    return total;
}

void odom::loop() {
    serial::update();

    float odom = distance * wheel_conversion / 1000.0 / num_magnets;
    float trip = trip_distance * wheel_conversion / 1000.0 / num_magnets;
    float tacho = sum_blocks() / (average_time / 1000.0) / num_magnets;
    float speed = tacho * wheel_conversion / 1000.0 * 3600;

    UPDATE_DISPLAY_FUNCTION(tacho, speed, odom, trip);
}

ISR(TIMER1_COMPA_vect) {
    static uint32_t distance_timer(0);
    distance_timer += block_time;

    // Process the time block
    distance += odom::time_blocks[odom::active_block];
    trip_distance += odom::time_blocks[odom::active_block];

    if (distance_timer >= distance_update_time) {
        PRINTLN("Saving Distance to EEPROM");
        rom::write(rom::Distance, distance);
        rom::write(rom::Trip, trip_distance);
        distance_timer = 0;
    }

    odom::active_block++;
    if (odom::active_block >= num_blocks) {
        odom::active_block = 0;
    }
    odom::time_blocks[odom::active_block] = 0;

    odom::sleep_timer += block_time;
    if (odom::sleep_timer > sleep_time) {
        rom::write(rom::Distance, distance);
        rom::write(rom::Trip, trip_distance);
        hardware::sleep();
    }
}