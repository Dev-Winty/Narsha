#include <Arduino.h>

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

char state = 0;

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
    TIMSK1 = 0x02;
}

void timerYDisable() {
    TIMSK3 = 0x00;
}

void timerYEnable() {
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

    countDistanceX = (int)(43.3 * 80);
    countDistanceY = (int)(25 * 80);
    
    halfPeriodX = 200;
    halfPeriodY = (int)(200 * 1.732);

    OCR1A = halfPeriodX * 2;
    OCR3A = halfPeriodY * 2;

    enableMotorX = ENABLE;
    enableMotorY = ENABLE;

    inturruptStoppedX = RUNNING;
    inturruptStoppedY = RUNNING;

    motorDirection(RIGHT);
    motorDirection(UP);

    state = 1;
}

void loop()
{

    if (state == 1) {
        if (inturruptStoppedX == STOPPED) {
            timerXDisable();
        }

        if (inturruptStoppedX == STOPPED) {
            timerXDisable();
        }

        if (inturruptStoppedX == STOPPED && inturruptStoppedY == STOPPED) {
            state = 2;


            timerYEnable();

            enableMotorX = DISABLE;
            enableMotorY = ENABLE;

        } 
    } else if (state == 2) {

        if (inturruptStoppedX == STOPPED) {
            timerXDisable();
        }

        if (inturruptStoppedX == STOPPED) {
            timerXDisable();
        }
        if (inturruptStoppedX == STOPPED && inturruptStoppedY == STOPPED) {
            state = 3;

            countDistanceX = (int)(43.3 * 80);
            countDistanceY = 0;

            halfPeriodX = 200;
            halfPeriodY = 0;

            OCR1A = halfPeriodX * 2;
            OCR3A = halfPeriodY * 2;

            inturruptStoppedX = RUNNING;
            inturruptStoppedY = STOPPED;

            motorDirection(LEFT);

            enableMotorX = ENABLE;
            enableMotorY = DISABLE;

            timerXEnable();

        } 
    } else if (state == 3) {
        // state = 4;

        // inturruptStoppedX = STOPPED;
        // inturruptStoppedY = STOPPED;

        // enableMotorX = DISABLE;
        // enableMotorY = DISABLE;

        // timerXDisable();
        // timerYDisable();
    }
    
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