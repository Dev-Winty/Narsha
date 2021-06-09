
void setup()
{
    pinMode(13, OUTPUT);
    digitalWrite(13, HIGH);
    
}

unsigned long cur_micros = 0;
unsigned long pre_micros = 0;
unsigned long cur_millis = 0;
unsigned long pre_millis = 0;
int count = 0;
int duty_count = 22;

void loop()
{
    cur_micros = micros();
    cur_millis = millis();

    if (cur_micros - pre_micros > 32) {
        pre_micros = cur_micros;

        if (count == 625) {
            count = 0;

            digitalWrite(7, HIGH);
        } else if (count == 22) {
            digitalWrite(7, LOW);
        }
    }

    if (cur_millis - pre_millis > duty_count) {
        pre_millis = cur_millis;

        duty_count++;
        if (duty_count > 72) {
            duty_count = 22;
        }
    }
}
