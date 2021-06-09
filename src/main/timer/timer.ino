#include <Arduino.h>

unsigned int count = 0;
int flag = 0;
unsigned long cur_millis = 0;
unsigned long pre_millis = 0;
int pwm_duty = 1;

void setup()
{
    DDRH |= (0x01 << 4);

    TCCR2A = 0x02;
    TCCR2B = 0x02;
    TCNT2 = 0;
    OCR2A = 200;
    TIMSK2 = 0x02;
}

void loop()
{
    cur_millis = millis();

    if (cur_millis - pre_millis > 20) {
        pre_millis = cur_millis;
        pwm_duty++;

        if (pwm_duty == 90) {
            pwm_duty = 0;
        }
    }
    // PORTH |= (0x01 << 4);
    // delay(1000);
    // PORTH &= ~(0x01 << 4);
    // delay(1000);
}



SIGNAL(TIMER2_OVF_vect) {
    // count++;
    // TCNT2 = 56;

    // if (count >= 10000) {

    //     count = 0;

    //     if (flag == 0) {
    //         flag = 1;
    //         PORTH |= (0x01 << 4);
    //     } else {
    //         PORTH &= ~(0x01 << 4);
    //         flag = 0;
    //     }
    // }
    // TCNT2 = 56;
    // count++;

    // if (count == 100) {
    //     count = 0;
    //     PORTH |= (0x01 << 4);
    // } else if (count == pwm_duty) {
    //     PORTH &= ~(0x01 << 4);
    // }

}

SIGNAL (TIMER2_COMPA_vect) {

    TCNT2 = 0;
    count++;

    if (count == 100) {
        count = 0;
        PORTH |= (0x01 << 4);
    } else if (count == pwm_duty) {
        PORTH &= ~(0x01 << 4);
    }

}