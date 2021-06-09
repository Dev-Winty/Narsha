#include <Arduino.h>

#define DATA 7
#define CLK 8
#define LATCH 9

unsigned char value74595 = 0x03;

void setup()
{
    Serial.begin(9600);

    for (int i = 0; i < 3; i++) {
        pinMode(DATA + i, OUTPUT);
    }
    spi74595(value74595);

}

int count = 0;
int countVariable = 0;
int pwmDuty = 1;

void loop()
{

    count++;

    if (count == 100) {
        count = 0;

        value74595 |= 0x01;
        spi74595(value74595);
    } else if (count == pwmDuty) {
        value74595 &= ~0x01;
        spi74595(value74595);
    }

    countVariable++;

    if (countVariable == 100) {
        countVariable = 0;

        pwmDuty++;

        if (pwmDuty == 100) {
            pwmDuty = 1;
        }
    }

    delayMicroseconds(100);

}

void spi74595(unsigned char data) {

    for (int i = 0; i < 8; i++) {
        if (data & (0x80 >> i)) {
            digitalWrite(DATA, HIGH);
        } else {
            digitalWrite(DATA, LOW);
        }
        digitalWrite(CLK, HIGH);
        digitalWrite(CLK, LOW);
    }
    digitalWrite(LATCH, HIGH);
    digitalWrite(LATCH, LOW);
}