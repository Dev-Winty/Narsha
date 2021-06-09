#include <Arduino.h>
#include <stdarg.h>

#define RS 3
#define RW 4
#define EN 5
#define DB0 6 
#define DB1 7 
#define DB2 8 
#define DB3 9 
#define DB4 10 
#define DB5 11
#define DB6 12 
#define DB7 13 
#define BACKLIGHT 14

enum charLCDLine{
    LINE1 = 1,
    LINE2,
    LINE3,
    LINE4
};

enum {
    DISABLE = 0,
    ENABLE
};

char buf[50];

char *lcdMenu[] = {
    ">1.PWM BL       ",
    " 2.PWM BL On/Off",
    " 3.Servo Motor  ",
    " 4. ADC         "
};


void setup()
{
    Serial.begin(9600);
    pinMode(BACKLIGHT, OUTPUT);
    digitalWrite(BACKLIGHT, HIGH);

    for (int i = 0; i < 11; i++) {
        pinMode(RS + i, OUTPUT);
    }

    digitalWrite(RS, HIGH);
    digitalWrite(RW, LOW);

    charLCDInst8(0x30);
    charLCDInst8(0x30);
    charLCDInst8(0x30);
    charLCDInst8(0x38);
    charLCDInst8(0x06);
    charLCDInst8(0x0C);
    charLCDInst8(0x80);
    charLCDInst8(0x01);

    delay(2);

    cursorSet(LINE1, 0);
    printfCharLCD("%s", lcdMenu[0]);
    cursorSet(LINE2, 0);
    printfCharLCD("%s", lcdMenu[1]);

}

int count = 0;
char linePos = LINE1;
char menuPos = LINE1;
char enterPos = LINE1;

unsigned long curMicros = 0;
unsigned long preMicros = 0;
unsigned long curMillis = 0;
unsigned long preMillis = 0;

int pwmCount = 0;
int pwmDuty = 1;

char toggleFlagLine1 = 0;
char toggleFlagLine2 = 0;

char backlightPwmEnable = 0;

void loop()
{
    curMicros = micros();
    curMillis = millis();

    if (backlightPwmEnable == 1) {

        if (curMicros - preMillis > 100) {
            preMicros = curMicros;

            pwmCount++;
            if (pwmCount == 100) {
                pwmCount = 0;
                digitalWrite(BACKLIGHT, HIGH);
            } else if (pwmCount == pwmDuty) {
                digitalWrite(BACKLIGHT, LOW);
            }
        }

        if (curMillis - preMillis > 50) {
            preMillis = curMillis;

            pwmDuty++;

            if (pwmDuty == 99) {
                pwmDuty = 1;
            }
        }
    } else {
        digitalWrite(BACKLIGHT, HIGH);
    }

    if (Serial.available()) {
        char inKey = Serial.read();

        if (inKey == 'w') {
            // Serial.println("up");
            if (linePos == LINE2) {
                linePos = LINE1;
                cursorSet(LINE2, 0);
                printfCharLCD(" ");
                cursorSet(LINE1, 0);
                printfCharLCD(">");
            } else {
                menuPos--;
                if (menuPos > 0) {
                    for (int i = 0; i < 2; i++) {
                        cursorSet(LINE1 + i, 0);
                        printfCharLCD("%s", lcdMenu[menuPos - 1 + i]);
                    }
                    cursorSet(LINE1, 0);
                    printfCharLCD(">");
                } else {
                    menuPos = 1;
                }
            }
        } else if (inKey == 's') {
            // Serial.println("down");
            if (linePos == LINE1) {
                linePos = LINE2;
                cursorSet(LINE1, 0);
                printfCharLCD(" ");
                cursorSet(LINE2, 0);
                printfCharLCD(">");
            } else {

                menuPos++;
                if (menuPos < 4) {
                    for (int i = 0; i < 2; i++) {
                        cursorSet(LINE1 + i, 0);
                        printfCharLCD("%s", lcdMenu[menuPos - 1 + i]);
                    }
                    cursorSet(LINE2, 0);
                    printfCharLCD(">");
                } else {
                    menuPos = 3;
                }
            } 
        } else if (inKey == ' ') {
            enterPos = linePos + menuPos - 1;

            if (enterPos == LINE1) {
                // Serial.println("1");
                if (toggleFlagLine1 == 0) {
                    toggleFlagLine1 = 1;
                    backlightPwmEnable = 1;
                } else {
                    toggleFlagLine1 = 0;
                    backlightPwmEnable = 0;
                }
            } else if (enterPos == LINE2) {
                // Serial.println("2");
                if (toggleFlagLine2 == 0) {
                    toggleFlagLine2 = 1;
                    backlightPwmEnable = DISABLE;
                    digitalWrite(BACKLIGHT, LOW);
                } else {
                    toggleFlagLine2 = 0;
                    backlightPwmEnable = ENABLE;
                    digitalWrite(BACKLIGHT, HIGH);
                }
            }
        }
    }

    // charLCDInst8(0xC0);

    // printfCharLCD("count : %d", count++);

    // delay(1000);
}

void cursorSet(char line, char column) {

    if (line == LINE1) {
        charLCDInst8(0x80 | column);
    } else if (line == LINE2) {
        charLCDInst8(0xC0 | column);
    } 
}

void charLCDInst8(unsigned char inst) {

    digitalWrite(RS, LOW);
   
    for (int i = 0; i < 8; i++) {
        if (inst & (0x01 << i)) {
            digitalWrite(DB0 + i, HIGH);
        } else {
            digitalWrite(DB0 + i, LOW);
        }
    }

    digitalWrite(EN, HIGH);
    digitalWrite(EN, LOW);

    digitalWrite(RS, HIGH);

    delayMicroseconds(50);
}

void charLCDData8(unsigned char inst) {

    for (int i = 0; i < 11; i++) {
        pinMode(RS + 1, OUTPUT);
    }

    for (int i = 0; i < 8; i++) {
        if (inst & (0x01 << i)) {
            digitalWrite(DB0 + i, HIGH);
        } else {
            digitalWrite(DB0 + i, LOW);
        }
    }

    digitalWrite(EN, HIGH);
    digitalWrite(EN, LOW);

    delayMicroseconds(50);
}

void charLCDString(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        //charLCDData8(*(str + i));
        charLCDData8(str[i]);
    }
}
void printfCharLCD(char *format, ...)
{
	char buf[512];
	va_list arglist;

	va_start(arglist, format);
	vsprintf(buf, format, arglist);
	va_end(arglist);
    charLCDString(buf);
}
