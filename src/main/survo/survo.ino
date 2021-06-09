#include <Arduino.h>

int survo_duty = 700;
int count = 0;
int duty_count = 22;
unsigned int count_1 = 0;

void setup()
{
    pinMode(7, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    // for (int j = 0; j < 50; j++) {
    //     for (int i = 0; i < 2; i++) {
    //         digitalWrite(7, HIGH);
    //         delayMicroseconds(servo_duty);
    //         digitalWrite(7, HIGH);
    //         delay(17);
    //         delayMicroseconds(3000 - servo_duty);
    //     }
    //     servo_duty += 32;
    //     if (servo_duty > 2300) {
    //         servo_duty = 700;
    //     }
    // }

    count++;

    if (count == 625) {
        count = 0;
        digitalWrite(7, HIGH);
    } else if (count == duty_count) {
        digitalWrite(7, LOW);
    }

    if (count_1 == 625) {
        count_1 = 0;
        if (duty_count == 72) {
            duty_count = 22;
        }
    }

    delayMicroseconds(32);
}