#include <Arduino.h>
#include <stdarg.h>

#define RS 3
#define RW 4
#define EN 5
#define DB4 10 
#define DB5 11
#define DB6 12 
#define DB7 13 
#define BACKLIGHT 14

void setup()
{
    Serial.begin(9600);

    pinMode(RS, OUTPUT);
    pinMode(RW, OUTPUT);
    pinMode(EN, OUTPUT);
    pinMode(BACKLIGHT, OUTPUT);
    digitalWrite(BACKLIGHT, HIGH);

    for (int i = 0; i < 4; i++) {
        pinMode(DB4 + i, OUTPUT);
    }

    digitalWrite(RS, HIGH);
    digitalWrite(RW, LOW);

    //8bit
    charLCDInst8Bit(0x30);
    charLCDInst8Bit(0x30);
    charLCDInst8Bit(0x30);
    charLCDInst8Bit(0x28);
    //4bit
    charLCDInst(0x28);
    charLCDInst(0x06);
    charLCDInst(0x0c);
    charLCDInst(0x80);
    charLCDInst(0x01);

    delay(2);
}

void loop()
{
    charLCDData(0x31);
    delay(1000);
}

void charLCDInst8Bit(unsigned char inst) {

    digitalWrite(RS, LOW);
    
    for (int i = 0; i < 4; i++) {

        if (inst & (0x10 << i)) {
            digitalWrite(DB4 + i, HIGH);
        } else {
            digitalWrite(DB4 + i, LOW);
        }
    }

    digitalWrite(EN, HIGH);
    digitalWrite(EN, LOW);

    digitalWrite(RS, HIGH);

    delayMicroseconds(50);
}

void charLCDInst4Bit(unsigned char inst) {

    digitalWrite(RS, LOW);
    
    for (int i = 0; i < 4; i++) {

        if (inst & (0x01 << i)) {
            digitalWrite(DB4 + i, HIGH);
        } else {
            digitalWrite(DB4 + i, LOW);
        }
    }

    digitalWrite(EN, HIGH);
    digitalWrite(EN, LOW);

    digitalWrite(RS, HIGH);

    delayMicroseconds(50);
}

void charLCDInst(unsigned char inst) {

    char upperByte = (inst & 0xf0) >> 4;
    char lowerByte = (inst & 0x0f);

    charLCDInst4Bit(upperByte);
    charLCDInst4Bit(lowerByte);
}
void charLCDData4Bit(unsigned char data) {
    
    for (int i = 0; i < 4; i++) {

        if (data & (0x01 << i)) {
            digitalWrite(DB4 + i, HIGH);
        } else {
            digitalWrite(DB4 + i, LOW);
        }
    }

    digitalWrite(EN, HIGH);
    digitalWrite(EN, LOW);

    delayMicroseconds(50);
}

void charLCDData(unsigned char data) {

    char upperByte = (data & 0xf0) >> 4;
    char lowerByte = (data & 0x0f);

    charLCDData4Bit(upperByte);
    charLCDData4Bit(lowerByte);
}