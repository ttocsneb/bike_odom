#include "serial.hpp"

#include "config.hpp"
#include "rom.hpp"

#include <Arduino.h>

const char OK = 0x06;
const char BAD = 0x15;

enum Rom {
    BLOCK_TIME = 'b',
    AVERAGE_TIME = 'a',
    NUM_MAGNETS = 'm',
    DIAMETER = 'd',
    SLEEP_TIME = 's',
    DISTANCE = 'D',
    DISTANCE_UPDATE_TIME = 'u'
};

enum Command {
    WRITE = 'w',
    READ = 'r',
    RESET = 'p'
};

void serial::setup() {
    Serial.begin(9600);

    PRINTLN("Hello World!");
}


void serial::update() {
    while (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n');
        if (String("wrp").indexOf(data[0]) == -1) {
            Serial.write(BAD);
            Serial.println();
            continue;
        }
        Command c = static_cast<Command>(data[0]);

        if (c == WRITE || c == READ) {

            if (String("bamdsDu").indexOf(data[1]) == -1) {
                Serial.write(BAD);
                Serial.println();
                continue;
            }
            Rom r = static_cast<Rom>(data[1]);
            uint8_t address;
            switch (r)
            {
            case BLOCK_TIME:
                address = rom::Block_Time;
                break;
            case AVERAGE_TIME:
                address = rom::Average_Time;
                break;
            case NUM_MAGNETS:
                address = rom::Num_Magnets;
                break;
            case DIAMETER:
                address = rom::Diameter;
                break;
            case SLEEP_TIME:
                address = rom::Sleep_Time;
                break;
            case DISTANCE:
                address = rom::Distance;
                break;
            case DISTANCE_UPDATE_TIME:
                address = rom::DistanceUpdateTime;
                break;
            }

            if (c == WRITE) {
                long value = data.substring(2).toInt();
                if (value == 0) {
                    Serial.write(BAD);
                    Serial.println();
                    continue;
                }
                rom::write(address, value);
                Serial.write(OK);
                Serial.println();
            } else {
                long value = rom::read_long(address);
                Serial.print(value);
                Serial.write(OK);
                Serial.println();
            }
        } else {
            Serial.write(BAD);
            Serial.println();
        }
    }
}