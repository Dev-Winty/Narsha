#include <Arduino.h>

#define ENC_A 11
#define ENC_B 10
#define ENC_BUTTON 16

#define X_DIR 21
#define X_STEP 15
#define X_STOP 18
#define XY_ENABLE 14
#define Y_DIR 13
#define Y_STEP 22
#define Y_STOP 19

enum encState {
    ENC_STATE_1 = 1,
    ENC_STATE_2,
    ENC_STATE_3,
    ENC_STATE_4
};

enum runStop{
    RUN = 0,
    STOP
};

enum direction {
    DIR_LEFT = 0,
    DIR_RIGHT,
    DIR_UP,
    DIR_DOWN
};

enum menuPos {
    MAIN_MENU_NONE = 0,
    MAIN_LEFT_STEP,
    MAIN_RIGHT_STEP,
    MAIN_SPEED_0_4,
    MAIN_SPEED_1,
    MAIN_SPEED_10,
    MAIN_SPEED_SET
};

int valueEncA = 0;
int valueEncB = 0;
int beforeValueEnc = 0;

char toggleFlagX = 0;
char toggleFlagY = 0;
int runStopX = STOP;
int runStopY = STOP;
double distance = 0.1;
int numOfStepCountX = 0;
int numOfStepCountY = 0;
int stepCountX = 0;
int stepCountY = 0;

char rightFlag = 0;
char leftFlag = 0;

encState state;
menuPos mainMenuPos;

int xStopValue = 0;
int yStopValue = 0;

char menuPosition = MAIN_MENU_NONE;

void mainMenu() {
    Serial.println("1. Left step");
    Serial.println("2. Right step");
    Serial.println("3. speed 0.4");
    Serial.println("4. speed 1");
    Serial.println("5. speed 10");
    Serial.println("6. speed set");
}

void setup()
{
    Serial.begin(9600);

    pinMode(ENC_A, INPUT);
    pinMode(ENC_B, INPUT);

    pinMode(X_DIR, OUTPUT);
    pinMode(X_STEP, OUTPUT);
    pinMode(XY_ENABLE, OUTPUT);
    pinMode(Y_DIR, OUTPUT);
    pinMode(Y_STEP, OUTPUT);
    pinMode(X_STOP, INPUT);
    pinMode(Y_STOP, INPUT);

    digitalWrite(XY_ENABLE, LOW);
    digitalWrite(X_DIR, HIGH);

    digitalWrite(ENC_A, HIGH);
    digitalWrite(ENC_B, HIGH);

    digitalWrite(X_STOP, HIGH);
    digitalWrite(Y_STOP, HIGH);

    TCCR1A = 0x00;
    TCCR1B = 0x0A;
    TCCR1C = 0x00;
    TCNT1 = 0;
    OCR1A = 400;
    TIMSK1 = 0x02;

    TCCR2A = 0x00;
    TCCR2B = 0x0A;
    TCNT2 = 0;
    OCR2A = 400;
    TIMSK2 = 0x02;

    mainMenu();
}

void directionX(char dir) {
    if (dir == DIR_LEFT) {
        digitalWrite(X_DIR, HIGH);
    } else {
        digitalWrite(X_DIR, LOW);
    }
}

void directionY(char dir) {
    if (dir == DIR_UP) {
        digitalWrite(Y_DIR, LOW);
    } else {
        digitalWrite(Y_DIR, HIGH);
    }
}

void loop()
{

    if (Serial.available()) {
        char keyValue = Serial.read();

        if (keyValue == '1') {

            mainMenuPos = MAIN_LEFT_STEP;
            // directionX(DIR_LEFT);
            // numOfStepCountX = distance * 80;
            // runStopX = RUN;
        } else if (keyValue == '2') {
            mainMenuPos = MAIN_RIGHT_STEP;

            // directionX(DIR_RIGHT);
            // numOfStepCountX = distance * 80;
            // runStopX = RUN;
        } else if (keyValue == '3') {
            mainMenuPos = MAIN_SPEED_0_4;
            distance = 0.4;
        } else if (keyValue == '4') {
            mainMenuPos = MAIN_SPEED_1;
            distance = 1;
        } else if (keyValue == '5') {
            mainMenuPos = MAIN_SPEED_10;
            distance = 10;
        } else if (keyValue == '6') {
            mainMenuPos = MAIN_SPEED_SET;
            Serial.println("select main speed set ");
        }
    }

    beforeValueEnc = state;

    valueEncA = digitalRead(ENC_A);
    valueEncB = digitalRead(ENC_B);

    if (valueEncA == 0 && valueEncB == 1) {
        state = ENC_STATE_1;
        // if (beforeValueEnc == ENC_STATE_2) {
        //     Serial.println("Right");
        // }
        // distance = 10;
        // numOfStepCount = (int)(distance * 80);
        // runStopX = RUN;
    } else if (valueEncA == 0 && valueEncB == 0) {
        state = ENC_STATE_2;
    } else if (valueEncA == 1 && valueEncB == 0) {
        state = ENC_STATE_3;
        // if (beforeValueEnc == ENC_STATE_2) {
        //     Serial.println("Left");
        // }
    } else if (valueEncA == 1 && valueEncB == 1) {
        state = ENC_STATE_4;

        rightFlag = 0;
        leftFlag = 0;
    }
//*******************************************************************************
    if (state == ENC_STATE_3 && beforeValueEnc == ENC_STATE_2) {
        if (rightFlag == 0) {

            if (mainMenuPos == MAIN_RIGHT_STEP) {
                rightFlag == 1;
                // Serial.println("Right");
                numOfStepCountX = (int)(distance * 80);
                runStopX = RUN;
            } else if (mainMenuPos == MAIN_SPEED_SET) {
                distance += 0.2;
                Serial.println("distance = " + String(distance));
            }
        }
    } else if (state == ENC_STATE_1 && beforeValueEnc == ENC_STATE_2) {
        if (leftFlag == 0) {

            leftFlag == 1;
            
            if (mainMenuPos == MAIN_LEFT_STEP) {
                numOfStepCountX = (int)(distance * 80);
                runStopX = RUN; 
            } else if (mainMenuPos == MAIN_SPEED_SET) {
                distance -= 0.2;
                Serial.println("distance = " + String(distance));
            }
        }
    }

    beforeValueEnc = state;
}

SIGNAL (TIMER1_COMPA_vect) {
    
    xStopValue = digitalRead(X_STOP);

    if (xStopValue == 0 && runStopX == RUN) {

        if (toggleFlagX == 0) {
            digitalWrite(X_STEP, HIGH);
            toggleFlagX = 1;
        } else {
            digitalWrite(X_STEP, LOW);
            toggleFlagX = 0;

            stepCountX++;

            if (stepCountX >= numOfStepCountX) {
                stepCountX = 0;
                runStopX = STOP;
            }
        }
    }
}
SIGNAL (TIMER2_COMPA_vect) {
    
    yStopValue = digitalRead(Y_STOP);

    if (yStopValue == 0 && runStopY == RUN) {

        if (toggleFlagY == 0) {
            digitalWrite(Y_STEP, HIGH);
            toggleFlagY = 1;
        } else {
            digitalWrite(Y_STEP, LOW);
            toggleFlagY = 0;

            stepCountY++;

            if (stepCountY >= numOfStepCountY) {
                stepCountY = 0;
                runStopY = STOP;
            }
        }
    }
}