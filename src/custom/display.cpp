#include "custom/display.hpp"
#include "config.hpp"
#include <stdint.h>
#include <Arduino.h>

#define ON HIGH
#define OFF LOW

#define MAX 255
#define TIMER_SCALE 5
#define timer_to_scale(x) (x == 1 ? 1 : (x == 2 ? 8 : (x == 3 ? 32 : (x == 4 ? 64 : (x == 5 ? 128 : (x == 6 ? 256 : (x == 7 ? 1024 : 0)))))))
#define UPDATE_TIME (120*4) // Hz
const uint16_t Timer_Steps = (F_CPU / timer_to_scale(TIMER_SCALE)) / UPDATE_TIME;


const uint8_t A = pA;
const uint8_t B = pB;
const uint8_t C = pC;
const uint8_t D = pD;
const uint8_t DP = pDP;

const uint8_t Dig[4] = {pD0, pD1, pD2, pD3};

const uint8_t Latch = pLATCH;

uint8_t digits[4];
bool dots[4];

void BCD(uint8_t val) {
    digitalWrite(A, val & 1);
    digitalWrite(B, (val & 2) != 0);
    digitalWrite(C, (val & 4) != 0);
    digitalWrite(D, (val & 8) != 0);
}

void display::setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);
    pinMode(DP, OUTPUT);

    pinMode(Dig[0], OUTPUT);
    pinMode(Dig[1], OUTPUT);
    pinMode(Dig[2], OUTPUT);
    pinMode(Dig[3], OUTPUT);

    pinMode(Latch, OUTPUT);
    digitalWrite(Latch, HIGH);

    // Setup the timer
    // Clear the timer registers
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0;
    // Set the length of the timer
    OCR2A = Timer_Steps;

    // Set the Mode to CTC (Clear Timer on Compare match)
    TCCR2A |= 1 << WGM21;

    // Set the Timer scale
    TCCR2B |= TIMER_SCALE;

    // Enable Timer2 to compare to OCR2A
    TIMSK2 = 1 << OCIE2A;

}

ISR(TIMER2_COMPA_vect) {
    static uint8_t active_digit(0);
    uint8_t last = active_digit;
    active_digit++;
    if (active_digit >= 4) {
        active_digit = 0;
    }
    BCD(digits[active_digit]);
    digitalWrite(Dig[last], OFF);
    // Disable the BCD Latch for a few microseconds
    digitalWrite(Latch, HIGH);
    digitalWrite(DP, !dots[active_digit]);
    digitalWrite(Dig[active_digit], ON);
    digitalWrite(Latch, LOW);
}


void display::setText(char* chars) {
    String text = chars;
    uint8_t d = 0;
    for (uint8_t i = 0; i < text.length() && d < 4; ++i) {
        digits[d] = 10; // 10 - 15: blank
        dots[d] = false;

        if (isDigit(text[i])) {
            digits[d] = String(text[i]).toInt();
        }
        if (text[i + 1] == '.') {
            dots[d] = true;
            ++i;
        }

        ++d;
    }
}

void display::display(float value, rom::Mode mode, rom::Unit unit) {
    char number[6] = "     ";
    dtostrf(value, 5, 1, number);

    PRINT("Display: '");

    if (mode != rom::TACHO && unit == rom::IMPERIAL) {
        char text[7] = {number[0], '.', number[1], number[2], number[3], number[4]};
        
        PRINT(text);
        setText(text);
    } else {
        PRINT(number);
        setText(number);
    }
    PRINTLN("'");

    digitalWrite(LED_BUILTIN, mode == rom::TACHO);
}

void display::onSleep() {
    digitalWrite(Dig[0], OFF);
    digitalWrite(Dig[1], OFF);
    digitalWrite(Dig[2], OFF);
    digitalWrite(Dig[3], OFF);
    digitalWrite(LED_BUILTIN, LOW);
    BCD(10);
    digitalWrite(DP, true);
}