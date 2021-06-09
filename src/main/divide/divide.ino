#include <Arduino.h>

unsigned int count = 0;
int flag = 0;
unsigned long cur_millis = 0;
unsigned long pre_millis = 0;
int pwm_duty = 1;

void setup()
{
    DDRH |= (0x01 << 4);

    TCCR2A = 0x03;
    TCCR2B = 0x05;
    TCNT2 = 6;
    OCR2A = 6 + 25;
    TIMSK2 = 0x03;
}

void loop()
{
    cur_millis = millis();

    if (cur_millis - pre_millis > 10) {
        pre_millis = cur_millis;
        pwm_duty++;

        if (pwm_duty == 255) {
            pwm_duty = 6;
        }
    }
    // PORTH |= (0x01 << 4);
    // delay(1000);
    // PORTH &= ~(0x01 << 4);
    // delay(1000);
}

SIGNAL (TIMER2_COMPA_vect) {
    PORTH &= ~(0x01 << 4);
}

SIGNAL(TIMER2_OVF_vect) {
    TCNT2 = 6;
    PORTH |= (0x01 << 4);

    OCR2A = pwm_duty;
}