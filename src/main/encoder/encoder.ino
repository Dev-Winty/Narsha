#include <Arduino.h>

#define ENCODER_A 10
#define ENCODER_B 11
#define ENCODER_BTN 16

void setup()
{
    pinMode(ENCODER_A, INPUT);
    pinMode(ENCODER_B, INPUT);
    pinMode(ENCODER_BTN, INPUT);
    Serial.begin(9600);

    digitalWrite(ENCODER_A, HIGH);
    digitalWrite(ENCODER_B, HIGH);
    digitalWrite(ENCODER_BTN, HIGH);
}

char flag_btn = 0;
char flag_up = 0;
char flag_down = 0;

unsigned long cur_millis = 0;
unsigned long pre_millis = 0;

void loop()
{
    cur_millis = millis();

    int in = digitalRead(ENCODER_BTN);

    // if (in == 1) {
    //     if (flag_up == 0) {
    //         Serial.println("up");
    //         flag_up = 1;
    //         flag_down = 0;
    //     }
    // } else if (in == 0) {
    //     if (flag_down == 0) {

    //         flag_up = 0;
    //         flag_down = 1;
    //         Serial.println("down");
    //     }
    // }

    // if (in == 1) {
    //     if (flag_btn == 0) {
    //         Serial.println("up");
    //         flag_btn = 1;
    //     }
    // } else if (in == 0) {
    //     if (flag_btn == 1) {
    //         Serial.println("down");
    //         flag_btn = 0;
    //     }
    // }
    // delay(10);

    if (cur_millis - pre_millis > 10) {
        pre_millis = cur_millis;
        if (in == 1 && flag_btn == 0) {
            Serial.println("up");
            flag_btn = 1;
        } else if (in == 0 && flag_btn == 1) {
            flag_btn = 0;
        }
    }
}