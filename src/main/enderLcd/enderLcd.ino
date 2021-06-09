#include <Arduino.h>
#include <stdarg.h>

void printf_(char *format, ...)
{
	char buf[512];
	va_list arglist;

	va_start(arglist, format);
	vsprintf(buf, format, arglist);
	va_end(arglist);
    gLCDString(buf);
}

#define CS 28
#define SCK 30
#define MOSI 17
#define ENC_A 10
#define ENC_B 11
#define ENC_BTN 16
#define HOTBED 12

char btnFlag = 0;

enum lineNumber{
    LINE1 = 1,
    LINE2,
    LINE3,
    LINE4
};

enum {
    ENC_STATE_1 = 1,
    ENC_STATE_2,
    ENC_STATE_3,
    ENC_STATE_4
};

enum curMenu {
    MAIN_MENU = 0,
    SUB_MENU_1,
    SUB_MENU_2,
    SUB_MENU_3,
    SUB_MENU_4,
    SUB_MENU_5,
    SUB_MENU_6,
};

enum {
    BACK = 4,
    HOME
};

char curPos = 0;
char windowPos = 0;
int enterPos = 0;

char bedTempEnable = 0;

char currentMenu = MAIN_MENU;

void shift8Bit(unsigned char data) {

    for (int i = 0; i < 8; i++) {
        if (data & (0x80 >> i)) {
            digitalWrite(MOSI, HIGH);
        } else {
            digitalWrite(MOSI, LOW);
        }

        digitalWrite(SCK, HIGH);
        digitalWrite(SCK, LOW);
    }

}

void gLCDInst(unsigned char inst) {

    unsigned char upperByte  = (inst & 0xf0);
    unsigned char lowerByte = (inst & 0x0f) << 4; 

    digitalWrite(CS, HIGH);

    shift8Bit(0xf8);
    shift8Bit(upperByte);
    shift8Bit(lowerByte);

    digitalWrite(CS, LOW);

    delayMicroseconds(100);
}

void gLCDData(unsigned char data)
{

    unsigned char upperByte  = (data & 0xf0);
    unsigned char lowerByte = (data & 0x0f) << 4; 

    digitalWrite(CS, HIGH);

    shift8Bit(0xfa);
    shift8Bit(upperByte);
    shift8Bit(lowerByte);

    digitalWrite(CS, LOW);

    delayMicroseconds(100);
}

void gLCDInit() {

    gLCDInst(0x30);
    gLCDInst(0x30);
    gLCDInst(0x30);
    gLCDInst(0x38);
    gLCDInst(0x06);

    gLCDInst(0x0c);
    gLCDInst(0x80);
    gLCDInst(0x01);

    delay(2);

}

char buf[50];

char *mainMenu[] = {
    " 1.Step Motor1  ",
    " 2.Step Motor2  ",
    " 3.Step Motor3  ",
    " 4.Step Motor4  ",
    " 5.Step Motor5  ",
    " 6.Step Motor6  "
};

char *subMenu[2][6] = {
    {
        " 1.Bed Temp    s",
        " 2.Sub1 Motor2 s",
        " 3.Sub1 Motor3 s",
        " 4.Sub1 Motor4 s",
        " 5.Back        s",
        " 6.Home        s"
    },
    {
        " 1.Sub2 Motor1 s",
        " 2.Sub2 Motor2 s",
        " 3.Sub2 Motor3 s",
        " 4.Sub2 Motor4 s",
        " 5.Back        s",
        " 6.Home        s"
    }
};

void printAllMenu(char windowPos, char curPos) {

    for (int i = 0; i < 4; i++) {
        curSet(LINE1 + i, 0);
        printf_("%s", mainMenu[i + windowPos]);
    }
    curSet(LINE1 + curPos , 0);
    gLCDData('>');
}

void printSubMenu(char currentMenu, char windowPos, char curPos) {
    for (int i = 0; i < 4; i++) {
        curSet(LINE1 + i, 0);
        printf_("%s", subMenu[currentMenu - 1][i + windowPos]);
    }
    curSet(LINE1 + curPos , 0);
    gLCDData('>');
}

void clearLCD() {
    for (int i = 0; i < 4; i++) {
        curSet(LINE1 + i, 0);
        printf_("                ");
    }
}

void setup()
{
    Serial.begin(9600);

    for (int i = 0; i < 50; i++) {
        buf[i] = 0;
    }

    pinMode(CS, OUTPUT);
    pinMode(SCK, OUTPUT);
    pinMode(MOSI, OUTPUT);

    pinMode(ENC_A, INPUT);
    pinMode(ENC_B, INPUT);

    pinMode(ENC_BTN, INPUT);

    pinMode(HOTBED, OUTPUT);

    digitalWrite(ENC_BTN, HIGH);

    digitalWrite(ENC_A, HIGH);
    digitalWrite(ENC_B, HIGH);

    digitalWrite(HOTBED, LOW);

    // while (1) {
    //     int hotBedValue = analogRead(A6);

    //     Serial.println(hotBedValue);
    //     delay(100);
    // }

//-----------------------------------------------
    gLCDInit();

//-----------------------------------------------

    printAllMenu(0, LINE1 - 1);
}

int count = 0;

char encState = 0;
char encStatePre = 0;

char leftFlag = 0;
char rightFlag = 0;

int tempCount = 0;

void loop()
{

    encFunc();

    if (bedTempEnable == 1) {
        tempCount++;
        if (tempCount > 1000) {
            tempCount = 0;
            int value = analogRead(A6);

            curSet(LINE1, 0);
            printf_("Bed Temp: %05d ", value);

            if (value < 910) {
                digitalWrite(HOTBED, LOW);
            } else {
                digitalWrite(HOTBED, HIGH);
            }
        }
    }

    delay(1);
    // curSet(LINE1, 0);
    // // sprintf(buf, "count = %d", count++);
    // // gLCDString(buf);

    // printf_("count = %d", count++);

    // delay(1000);
}

void encBtnClick() {

    if (digitalRead(ENC_BTN) == 0) {

        if (currentMenu != MAIN_MENU && enterPos == BACK) {

        } else if (currentMenu != MAIN_MENU && enterPos == HOME) {
            currentMenu = MAIN_MENU;

            enterPos = 0;
            curPos = 0;
            windowPos = 0;

            printAllMenu(windowPos, curPos);

        } else if (currentMenu == SUB_MENU_1) {
            clearLCD();
            if (bedTempEnable == 0) {
                clearLCD();

                bedTempEnable = 1;
                digitalWrite(HOTBED, HIGH);
                curSet(LINE1, 0);
                printf_("Bed Temp: %d");
            } else if (bedTempEnable == 1) {
                bedTempEnable = 0;

                digitalWrite(HOTBED, LOW);
            }
        } else {
            currentMenu = enterPos + 1;

            enterPos = 0;
            curPos = 0;
            windowPos = 0;

            printSubMenu(currentMenu, windowPos, curPos);
        }
    }

}

void gLCDString(char *str) {
    for (int i = 0; i < strlen(str); i++) {
        gLCDData(*(str + i));
    }
}

void curSet(char line, char column) {
    if (line == LINE1) {
        gLCDInst(0x80 + column);
    } else if (line == LINE2) {
        gLCDInst(0x80 + 0x10 + column);
    } else if (line == LINE3) {
        gLCDInst(0x80 + 0x08 + column);
    } else if (line == LINE4) {
        gLCDInst(0x80 + 24 + column);
    }
}

void encFunc() {

    int encA = digitalRead(ENC_A);
    int encB = digitalRead(ENC_B);

    if (encA == 0 && encB == 1) {
        encState = ENC_STATE_1; 
    } else if (encA == 0 && encB == 0) {
        encState = ENC_STATE_2;
    } else if (encA == 1 && encB == 0) {
        encState = ENC_STATE_3;
    } else if (encA == 1 && encB == 1) {
        encState = ENC_STATE_4;
        leftFlag = 0;
        rightFlag = 0;
    }

    if (encState == ENC_STATE_3 && encStatePre == ENC_STATE_2) {
        if (rightFlag == 0) {
            rightFlag = 1;
            // curSet(LINE1, 0);
            // printf_("count = %3d", ++count);

            curSet(LINE1 + curPos, 0);
            gLCDData(' ');

            curPos++;
            if (curPos < 4) {
                curSet(LINE1 + curPos, 0);
                gLCDData('>');
            } else if (curPos >= 4) {
                curPos = 3;

                windowPos++;
                if (windowPos <= 2) {
                    if (currentMenu == MAIN_MENU) {
                        printAllMenu(windowPos, curPos);
                    } else {
                        printSubMenu(currentMenu, windowPos, curPos);
                    }
                } else {
                    windowPos = 2;
                    curSet(LINE1 + curPos, 0);
                    gLCDData('>');
                }
            }
        }
        enterPos = windowPos + curPos;
    }
    else if (encState == ENC_STATE_1 && encStatePre == ENC_STATE_2)
    {
        if (leftFlag == 0) {
            leftFlag = 1;
            // curSet(LINE1, 0);
            // printf_("count = %3d", --count);

            curSet(LINE1 + curPos, 0);
            gLCDData(' ');

            curPos--;
            if (curPos > 0) {
                curSet(LINE1 + curPos, 0);
                gLCDData('>');
            } else if (curPos <= 0) {
                curPos = 0;

                windowPos--;
                if (windowPos >= 0) {
                    if (currentMenu == MAIN_MENU) {
                        printAllMenu(windowPos, curPos);
                    } else {
                        printSubMenu(currentMenu, windowPos, curPos);
                    }
                } else {
                    windowPos = 0;
                    curSet(LINE1 + curPos, 0);
                    gLCDData('>');
                }
            }
        }
        enterPos = windowPos + curPos;
    }

    encStatePre = encState;

    //-------------------------------------------

    int encBtn = digitalRead(ENC_BTN);

    if (encBtn == 0) {
        if (btnFlag == 0) {
            // Serial.println("down");
            btnFlag = 1;
            encBtnClick();
        }
    } else {
        // Serial.println("up");
        btnFlag = 0;
    }
}