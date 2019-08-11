#include "led/display.hpp"
#include "config.hpp"
#include <stdint.h>
#include <Arduino.h>

#define ON HIGH
#define OFF LOW

const uint8_t A = 4;
const uint8_t B = 5;
const uint8_t C = 6;
const uint8_t D = 7;
const uint8_t DP = A4;

const uint8_t Dig[4] = {A0, A1, A2, A3};

const uint8_t Latch = A5;

uint8_t digits[4];
bool dots[4];

void BCD(uint8_t val, bool dp) {
    digitalWrite(A, (val & 0b0001) != 0);
    digitalWrite(B, (val & 0b0010) != 0);
    digitalWrite(C, (val & 0b0100) != 0);
    digitalWrite(D, (val & 0b1000) != 0);
    digitalWrite(DP, dp);
}

void display::setup() {
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
}

void display::update() {
    static uint8_t active_digit(0);
    digitalWrite(Dig[active_digit], OFF);
    digitalWrite(Latch, HIGH);

    active_digit++;
    if (active_digit >= 4) {
        active_digit = 0;
    }

    BCD(digits[active_digit], dots[active_digit]);

    digitalWrite(Latch, LOW);
    digitalWrite(Dig[active_digit], ON);

}

void setText(String text) {
    uint8_t d = 0;
    for (uint8_t i = 0; i < text.length() && d < 4; ++i) {
        digits[d] = 10; // 10 - 15: blank
        dots[d] = false;

        if (isDigit(text[i])) {
            digits[d] = String(text[i]).toInt();

            // If the next character is a '.' move to the next character,
            // so the dot can be placed on this digit.
            if (text[i + 1] == '.') {
                ++i;
            }
        }
        if (text[i] == '.') {
            dots[d] = true;
        }

        ++d;
    }
}

void display::display(float value, rom::Mode mode, rom::Unit unit) {
    double number = value / 10.0;
    char display[8];
    sprintf(display, "%4.1f", number);

    PRINT("Display: '");
    PRINT(display);
    PRINTLN("'");

    setText(display);
}

void display::onSleep() {
    digitalWrite(Dig[0], OFF);
    digitalWrite(Dig[1], OFF);
    digitalWrite(Dig[2], OFF);
    digitalWrite(Dig[3], OFF);
}