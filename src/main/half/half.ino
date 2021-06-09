#include <Arduino.h>

int count = 0;
unsigned long cur_millis = 0;
unsigned long pre_millis = 0;
int pwmDuty = 1;

void setup()
{
    pinMode(13, OUTPUT);

    Serial.begin(9600);

    TCCR2A = 0x00;
    TCCR2B = 0x02;
    TCNT2 = 256 - 200;
    TIMSK2 = 0x01;

}

void loop()
{
    cur_millis = millis();

    if (cur_millis - pre_millis > 10) {
        pre_millis = cur_millis;

        pwmDuty++;
        if (pwmDuty == 99) {
            pwmDuty = 1;
        }
    }
    
}

SIGNAL (TIMER2_OVF_vect) {
    TCNT2 = 256 - 200;

    count++;
    if (count == 100) {
        count = 0;
        digitalWrite(13, HIGH);
    } else if (count == pwmDuty) {
        digitalWrite(13, LOW);
    }
}