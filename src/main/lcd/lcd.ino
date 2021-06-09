#include <Arduino.h>

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    int a0Value = analogRead(A0);
    float voltage = ((float)a0Value / 1024.0f) * 5.0f;

    //Serial.println("저항: " + a0Value);
    Serial.println(voltage);

    delay(10);
}