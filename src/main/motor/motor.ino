#include <Arduino.h>

#define X_DIR 21
#define X_STEP 15
#define X_STOP 18

#define Y_DIR 23
#define Y_STEP 22
#define Y_STOP 19

#define XY_EN 14
enum {
    STOP = 0,
    RUN
};

enum {
    RUNNING = 0,
    STOPPED
};

enum {
    DIR_LEFT = 0,
    DIR_RIGHT
};

enum {
    DIR_UP = 0,
    DIR_DOWN
};

enum {
    STATE_NONE = 0,
    STATE_RIGHT,
    STATE_LEFT,
    STATE_UP,
    STATE_DOWN,
    STATE_CROSS1,
    STATE_CROSS2
};

int step_count_x = 0;
int stop_run_flag_x = STOP;
int how_many_count_x = 0;
int distance_x = 0;
char toggle_flag_x = 0;
char x_stopped = STOPPED;

int step_count_y = 0;
int stop_run_flag_y = STOP;
int how_many_count_y = 0;
int distance_y = 0;
char toggle_flag_y = 0;
char y_stopped = STOPPED;

int motor_state = STATE_NONE;

void x_direction(char dir) {
    if (dir == DIR_LEFT) digitalWrite(X_DIR, HIGH);
    else if (dir == DIR_RIGHT) digitalWrite(X_DIR, LOW);
}

void y_direction(char dir) {
    if (dir == DIR_UP) digitalWrite(Y_DIR, HIGH);
    else if (dir == DIR_DOWN) digitalWrite(Y_DIR, LOW);
}

void init_func() {
    distance_x = 50;
    how_many_count_x = distance_x * 80;

    distance_y = 50;
    how_many_count_y = distance_y * 80;

    x_stopped = RUNNING;
    y_stopped = RUNNING;

    motor_state = STATE_RIGHT;

    x_direction(DIR_RIGHT);
    y_direction(DIR_DOWN);

    stop_run_flag_x = RUN;
    stop_run_flag_y = STOP;
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    pinMode(X_DIR, OUTPUT);
    pinMode(X_STEP, OUTPUT);
    pinMode(X_STOP, INPUT);

    digitalWrite(X_DIR, LOW);

    pinMode(Y_DIR, OUTPUT);
    pinMode(Y_STEP, OUTPUT);
    pinMode(Y_STOP, INPUT);

    digitalWrite(Y_DIR, LOW);

    pinMode(XY_EN, OUTPUT);

    digitalWrite(XY_EN, LOW);

    TCCR2A = 0x02;
    TCCR2B = 0x04;
    TCNT2 = 0;
    OCR2A = 50;
    TIMSK2 = 0x02;

    init_func();
}

void loop() {
    if (motor_state == STATE_RIGHT) {
        if (x_stopped == STOPPED) {
            Serial.println("X Motor Stopped!!");

            motor_state = STATE_DOWN;

            y_direction(DIR_DOWN);
            y_stopped = RUNNING;
            stop_run_flag_y = RUN;
        }
    } else if (motor_state == STATE_DOWN) {
        if (y_stopped == STOPPED) {
            Serial.println("Y Motor Stopped!!");

            motor_state = STATE_LEFT;

            x_direction(DIR_LEFT);
            x_stopped = RUNNING;
            stop_run_flag_x = RUN;
        }
    } else if (motor_state == STATE_LEFT) {
        if (x_stopped == STOPPED) {
            Serial.println("X Motor Stopped!!");

            motor_state = STATE_UP;

            y_direction(DIR_UP);
            y_stopped = RUNNING;
            stop_run_flag_y = RUN;
        }
    } else if (motor_state == STATE_UP) {
        if (y_stopped == STOPPED) {

            motor_state = STATE_CROSS1;

            y_direction(DIR_DOWN);
            y_stopped = RUNNING;
            stop_run_flag_y = RUN;

            x_direction(DIR_RIGHT);
            x_stopped = RUNNING;
            stop_run_flag_x = RUN;

        }
    } else if (motor_state == STATE_CROSS1) {
        if (y_stopped == STOPPED ) {

            motor_state = STATE_CROSS2;

            x_direction(DIR_RIGHT);
            x_stopped == RUNNING;
            stop_run_flag_x = RUN;
        }
    } else if (motor_state == STATE_CROSS2) {
        if (x_stopped == STOPPED) {

            motor_state == STATE_NONE;

            y_direction(DIR_UP);
            y_stopped = RUNNING;
            stop_run_flag_y = RUN;

            x_direction(DIR_LEFT);
            x_stopped = RUNNING;
            stop_run_flag_x = RUN;
        }
    }
}

SIGNAL(TIMER2_COMPA_vect) {
    //-----------------X축---------------------
    if (stop_run_flag_x == RUN) {
        if (toggle_flag_x == 0) {
            toggle_flag_x = 1;
            digitalWrite(X_STEP, HIGH);
        } else {
            toggle_flag_x = 0;
            digitalWrite(X_STEP, LOW);

            step_count_x++;
            if (step_count_x > how_many_count_x) {
                step_count_x = 0;
                stop_run_flag_x = STOP;
                x_stopped = STOPPED;
            }
        }
    }

    //---------------Y축--------------------

    if (stop_run_flag_y == RUN) {
        if (toggle_flag_y == 0) {
            toggle_flag_y = 1;
            digitalWrite(Y_STEP, HIGH);
        } else {
            toggle_flag_y = 0;
            digitalWrite(Y_STEP, LOW);

            step_count_y++;
            if (step_count_y > how_many_count_y) {
                step_count_y = 0;
                stop_run_flag_y = STOP;
                y_stopped = STOPPED;
            }
        }
    }
}