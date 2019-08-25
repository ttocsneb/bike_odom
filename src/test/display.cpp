#include "config.hpp"
#ifdef TEST_DISPLAY
#include <Arduino.h>

// Make sure the display is working properly


#define pA 4
#define pB 5
#define pC 6
#define pD 7
#define pDP A5

#define pD0 A0
#define pD1 A1
#define pD2 A2
#define pD3 A3

#define pLATCH A4

const uint8_t A = pA;
const uint8_t B = pB;
const uint8_t C = pC;
const uint8_t D = pD;
const uint8_t DP = pDP;

const uint8_t Dig[4] = {pD0, pD1, pD2, pD3};

const uint8_t Latch = pLATCH;

void setup() {
    Serial.begin(9600);
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

    digitalWrite(A, 0);
    digitalWrite(B, 0);
    digitalWrite(C, 1);
    digitalWrite(D, 0);

    digitalWrite(DP, LOW);
    digitalWrite(Dig[0], 1);
    digitalWrite(Dig[1], 0);
    digitalWrite(Dig[2], 0);
    digitalWrite(Dig[3], 0);
}

const uint16_t Update_Time = 1000000 / (200*4); // 200Hz display rate

void loop() {
    static uint8_t count(0);
    static uint8_t digits(0);

    digitalWrite(A, count & 1);
    digitalWrite(B, (count & 2) != 0);
    digitalWrite(C, (count & 4) != 0);
    digitalWrite(D, (count & 8) != 0);

    digitalWrite(Dig[0], digits == 0);
    digitalWrite(Dig[1], digits == 1);
    digitalWrite(Dig[2], digits == 2);
    digitalWrite(Dig[3], digits == 3);

    ++count;
    ++digits;

    if (count >= 10) {
        count = 0;
    }

    if (digits >= 4) {
        digits = 0;
    }

    delay(250);

    // delayMicroseconds(Update_Time);
}

#endif