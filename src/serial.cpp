#include "serial.hpp"

#include "config.hpp"
#include "rom.hpp"
#include "odom.hpp"

#include <Arduino.h>

const char OK = 'G';//0x06;
const char BAD = 'B';//0x15;

enum Rom {
    BLOCK_TIME = 'b',
    AVERAGE_TIME = 'a',
    NUM_MAGNETS = 'm',
    DIAMETER = 'd',
    SLEEP_TIME = 's',
    DISTANCE = 'D',
    DISTANCE_UPDATE_TIME = 'u',
    TRIP = 'o'
};

enum Command {
    WRITE = 'w',
    READ = 'r',
    RESET = 't',
    SETUP = 'p'
};

void serial::setup() {
    Serial.begin(115200);

    PRINTLN("Hello World!");
}

void status(char status) {
    Serial.write(status);
    Serial.println();
}


void serial::update() {
    while (Serial.available() > 0) {
        String data = Serial.readStringUntil('\n');
        if (String("wrpt").indexOf(data[0]) == -1) {
            status(BAD);
            continue;
        }
        Command c = static_cast<Command>(data[0]);

        if (c == WRITE || c == READ) {

            if (String("bamdsDuo").indexOf(data[1]) == -1) {
                status(BAD);
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
            case TRIP:
                address = rom::Trip;
                break;
            }

            if (c == WRITE) {
                long value = data.substring(2).toInt();
                if (value == 0) {
                    status(BAD);
                    continue;
                }
                rom::write(address, value);
                status(OK);
            } else {
                long value = rom::read_long(address);
                Serial.print(value);
                status(OK);
            }
        } else if (c == SETUP) {
            odom::setup();
            status(OK);
        }else {
            status(BAD);
        }
    }
}