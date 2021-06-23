#include <Arduino.h>
#include "gcode_sample_480.h"

#define X_DIR 21
#define X_STEP 15
#define XY_ENABLE 14
#define Y_DIR 23
#define Y_STEP 22

enum {
    RUNNING = 0,
    STOPPED
};

enum {
    DISABLE = 0,
    ENABLE
};

enum dir {
    LEFT = 0,
    RIGHT,
    UP,
    DOWN
};

char toggleStepX = 0;
char toggleStepY = 0;

int stepCountX = 0;
int stepCountY = 0;

char enableMotorX = DISABLE;
char enableMotorY = DISABLE;

int countDistanceX = 0;
int countDistanceY = 0;

int halfPeriodX = 0;
int halfPeriodY = 0;

char inturruptStoppedX = STOPPED;
char inturruptStoppedY = STOPPED;

int state = 0;

void motorDirection(char dir) {
    if (dir == LEFT) {
        digitalWrite(X_DIR, HIGH);
    } else if (dir == RIGHT) {
        digitalWrite(X_DIR, LOW);
    } else if (dir == UP) {
        digitalWrite(Y_DIR, LOW);
    } else if (dir == DOWN) {
        digitalWrite(Y_DIR, HIGH);
    }
}

void timerXDisable() {
    TIMSK1 = 0x00;
}

void timerXEnable() {
    TCNT1 = 0;
    TIMSK1 = 0x02;
}

void timerYDisable() {
    TIMSK3 = 0x00;
}

void timerYEnable() {
    TCNT3 = 0;
    TIMSK3 = 0x02;
}

void motorXStart(float distance, int stepPeriod, char direction) {

    countDistanceX = (int)(distance * 80);

    halfPeriodX = stepPeriod;

    OCR1A = halfPeriodX  * 2;

    inturruptStoppedX = RUNNING;

    motorDirection(direction);
}

void motorYStart(float distance, int stepPeriod, char direction) {

    countDistanceY = (int)(distance * 80);

    halfPeriodY = stepPeriod;

    OCR3A = halfPeriodY  * 2;

    inturruptStoppedY = RUNNING;

    motorDirection(direction);
}

void motorXYEnable(char xEnable, char yEnable) {
    if (xEnable == ENABLE) {
        timerXEnable();
    }

    if (yEnable == ENABLE) {
        timerYEnable();
    }
    enableMotorX = xEnable;
    enableMotorY = yEnable;
}

void motorInit() {
    state = 1;

    // motorXStart(43.3, 200, RIGHT);
    // motorYStart(25, 200 * 1.732, UP);
    // motorXYEnable(ENABLE, ENABLE);
}

void motorMove(double startX, double startY, double endX, double endY, int speed) {
    float xDist = endX - startX;
    float yDist = endY - startY;

    char dirX;
    char dirY;

    int periodX;
    int periodY;

    periodX = 200;
    periodY = 200;

    double crossSpeed = (1000000.0 / (((double)speed / 60.0) * 80.0)) / 2.0;
    double theta = atan2(yDist, xDist);

    if (yDist != 0 && xDist != 0) {
        periodY = (int)(200.0 * (xDist / yDist));
    } 

    periodX = abs(periodX);
    periodY = abs(periodY);

    char moveEnableX;
    char moveEnableY;

    if (xDist > 0) {
        dirX = RIGHT;
    } else if (xDist < 0) {
        dirX = LEFT;
    }

    if (yDist > 0) {
        dirY = UP;
    } else if (yDist < 0){
        dirY = DOWN;
    }

    if (xDist == 0) {
        moveEnableX = DISABLE;
    } else {
        moveEnableX = ENABLE;
    }

    if (yDist == 0) {
        moveEnableY = DISABLE;
    } else {
        moveEnableY = ENABLE;
    }

    xDist = abs(xDist);
    yDist = abs(yDist);


    if (xDist != 0) {
        motorXStart((float)xDist, periodX, dirX);
    } 
    if (yDist != 0) {
        motorYStart((float)yDist, periodY, dirY);
    }

    motorXYEnable(moveEnableX, moveEnableY);

}

void setup()
{

    Serial.begin(9600);

    pinMode(X_DIR, OUTPUT);
    pinMode(X_STEP, OUTPUT);

    pinMode(XY_ENABLE, OUTPUT);

    pinMode(Y_DIR, OUTPUT);
    pinMode(Y_STEP, OUTPUT);

    digitalWrite(XY_ENABLE, LOW);

    TCCR1A = 0x00;
    TCCR1B = 0x0A;
    TCCR1C = 0x00;
    TCNT1 = 0x00;
    TIMSK1 = 0x02;

    TCCR3A = 0x00;
    TCCR3B = 0x0A;
    TCCR3C = 0x00;
    TCNT3 = 0x00;
    TIMSK3 = 0x02;

    // motorInit();

    // state = 0;
    // motorMove(0, 0, 50, 30);

}

double gCodeArray[4][2] = {
    {0, 0},
    {50, 0},
    {50, 50},
    {0, 50}
};

int preState = 0;

void loop()
{

    // state = 0;
    // motorMove(0.0, 0.0, (double)xy_pos[state][0], (double)xy_pos[state][1]);

    // preState = state;
    // state++

    while (1) {
        if (inturruptStoppedX == STOPPED) {
            timerXDisable();
        }

        if (inturruptStoppedX == STOPPED) {
            timerXDisable();
        }

        if (inturruptStoppedX == STOPPED && inturruptStoppedY == STOPPED) {

            // motorYStart(25, 200 * 1.732, DOWN);
            // motorXYEnable(DISABLE, ENABLE);

            // motorMove(gCodeArray[preState][0], gCodeArray[preState][1], gCodeArray[state][0], gCodeArray[state][1]);
            motorMove((double)xy_pos[preState][0], (double)xy_pos[preState][1], (double)xy_pos[state][0], (double)xy_pos[state][1], 1125);

            preState = state;
            state++;

            if (state == 480) {
                state = 0;
            }
        } 
    }

    // if (state == 1) {
    //     if (inturruptStoppedX == STOPPED) {
    //         timerXDisable();
    //     }

    //     if (inturruptStoppedX == STOPPED) {
    //         timerXDisable();
    //     }

    //     if (inturruptStoppedX == STOPPED && inturruptStoppedY == STOPPED) {

    //         // motorYStart(25, 200 * 1.732, DOWN);
    //         // motorXYEnable(DISABLE, ENABLE);

    //         delay(1000);
    //         motorMove(0, 0, 0, -30);
    //         state = 2;
    //     } 
    // } else if (state == 2) {

    //     if (inturruptStoppedX == STOPPED) {
    //         timerXDisable();
    //     }

    //     if (inturruptStoppedX == STOPPED) {
    //         timerXDisable();
    //     }
    //     if (inturruptStoppedX == STOPPED && inturruptStoppedY == STOPPED) {

    //         // motorXStart(43.3, 200, LEFT);
    //         // motorXYEnable(ENABLE, DISABLE);

    //         // motorMove(30, 0, 0, 0);
    //         state = 3;
    //     } 
    // } else if (state == 3) {
    //     // state = 4;

    //     // inturruptStoppedX = STOPPED;
    //     // inturruptStoppedY = STOPPED;

    //     // enableMotorX = DISABLE;
    //     // enableMotorY = DISABLE;

    //     // timerXDisable();
    //     // timerYDisable();
    //     if (inturruptStoppedX == STOPPED) {
    //         timerXDisable();
    //     }

    //     if (inturruptStoppedX == STOPPED) {
    //         timerXDisable();
    //     }
    //     if (inturruptStoppedX == STOPPED && inturruptStoppedY == STOPPED) {

    //         // motorXStart(43.3, 200, RIGHT);
    //         // motorYStart(25, 200 * 1.732, UP);
    //         // motorXYEnable(ENABLE, ENABLE);
    //         motorMove(0, 0, 50, 30);
    //         state = 1;
    //     } 
    // }
    
}

SIGNAL(TIMER1_COMPA_vect)
{

    if (enableMotorX == ENABLE) {
        if (toggleStepX == 0) {

            digitalWrite(X_STEP, HIGH);
            toggleStepX = 1;
        } else {
            toggleStepX = 0;

            stepCountX++;
            if (stepCountX > countDistanceX) {
                digitalWrite(X_STEP, LOW);
                enableMotorX = DISABLE;
                inturruptStoppedX = STOPPED;
                stepCountX = 0;
            } else {
                digitalWrite(X_STEP, LOW);
            }
        }
    }
}

SIGNAL(TIMER3_COMPA_vect)
{

    if (enableMotorY == ENABLE) {
        if (toggleStepY == 0) {

            digitalWrite(Y_STEP, HIGH);
            toggleStepY = 1;
        } else {
            toggleStepY = 0;

            stepCountY++;
            if (stepCountY > countDistanceY) {
                digitalWrite(Y_STEP, LOW);
                enableMotorY = DISABLE;
                inturruptStoppedY = STOPPED;
                stepCountY = 0;
            } else {
                digitalWrite(Y_STEP, LOW);
            }
        }
    }
}