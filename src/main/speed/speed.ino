#include <math.h>

#include "gcode_sample_480.h"
#include "gcode_sample_200_string.h"

#define X_ST0P 18
#define X_DIR 21
#define X_STEP 15

#define Y_STOP 19
#define Y_DIR 23
#define Y_STEP 22
#define XY_EN 14

enum {
  DISABLE = 0,
  ENABLE
};

enum {
  RUNNING = 0,
  STOPPED
};

enum dir {
  LEFT = 0,
  RIGHT,
  UP,
  DOWN
};


char toggle_step_x = 0;
char toggle_step_y = 0;
int step_count_x = 0;
int step_count_y = 0;

char enable_motor_x = 0;
char enable_motor_y = 0;

int count_distance_x = 0;
int count_distance_y = 0;

int helf_period_x = 0;
int helf_period_y = 0;

char interrupt_stopped_x = STOPPED;
char interrupt_stopped_y = STOPPED;

void motor_direction(char dir) {
  if (dir == LEFT) {
    digitalWrite(X_DIR, HIGH);
  }
  else if (dir == RIGHT) {
    digitalWrite(X_DIR, LOW);
  }
  else if (dir == UP) {
    digitalWrite(Y_DIR, LOW);
  }
  else if (dir == DOWN) {
    digitalWrite(Y_DIR, HIGH);
  }
}

void timer_x_enable() {
  TCNT1 = 0;
  TIMSK1 = 0x02;
}

void timer_x_disable() {
  TIMSK1 = 0x00;
}

void timer_y_enable() {
  TCNT3 = 0;
  TIMSK3 = 0x02;
}

void timer_y_disable() {
  TIMSK3 = 0x00;
}

char state = 0;

void motor_x_start(float distance, int step_period, char left_right) {
  count_distance_x = (int)(distance * 80);
  helf_period_x = step_period;
  OCR1A = helf_period_x * 2;
  TCNT1 = 0;
  interrupt_stopped_x = RUNNING;
  motor_direction(left_right);
}

void motor_y_start(float distance, int step_period, char up_down) {
  count_distance_y = (int)(distance * 80);
  helf_period_y = step_period;
  OCR3A = helf_period_y * 2;
  TCNT3 = 0;
  interrupt_stopped_y = RUNNING;
  motor_direction(up_down);
}

void motor_x_y_enable(char x_enable, char y_enable) {
  if (x_enable == ENABLE) {
    timer_x_enable();
  }
  if (y_enable == ENABLE) {
    timer_y_enable();
  }
  enable_motor_x = x_enable;
  enable_motor_y = y_enable;
}

void motor_init() {
  motor_x_start(43.3, 200, RIGHT);
  motor_y_start(25, 200 * 1.732, UP);
  motor_x_y_enable(ENABLE, ENABLE);

  state = 1;
}

//void motor_move(double start_x, double start_y, double end_x, double end_y, int speed) {
//  char dir_x;
//  char dir_y;
//
//  char move_enable_x;
//  char move_enable_y;
//
//  int period_x;
//  int period_y;
//
//  double x_dist = end_x - start_x;
//  double y_dist = end_y - start_y;
//
//  //-------------------------------------
//  if (x_dist > 0) dir_x = RIGHT;
//  else if (x_dist < 0) dir_x = LEFT;
//
//  if (y_dist > 0) dir_y = UP;
//  else if (y_dist < 0) dir_y = DOWN;
//  //-------------------------------------
//  if (x_dist == 0) move_enable_x = DISABLE;
//  else move_enable_x = ENABLE;
//
//  if (y_dist == 0) move_enable_y = DISABLE;
//  else move_enable_y = ENABLE;
//  //-------------------------------------
//
//  period_x = 200;
//  period_y = 200;
//
//  //F1125 -> half_period = 333
//  double half_period = (double)((1000000.0 / (((double)speed / 60.0) * 80)) / 2);
//  //Serial.println((int)half_period);
//
//  double theta = atan2(y_dist, x_dist);
//  //Serial.println(theta,5);
//
//  //  if(y_dist != 0 && x_dist != 0)
//  //    period_y = (int)200.0 * (x_dist/y_dist);
//  //
//  //  Serial.println("period_x = " + String(period_x));
//  //  Serial.println("period_y = " + String(period_y));
//  //
//  //  period_x = abs(period_x);
//  //  period_y = abs(period_y);
//  x_dist = abs(x_dist);
//  y_dist = abs(y_dist);
//
//  if (x_dist != 0) {
//    period_x = half_period * (1 / cos(theta));
//    motor_x_start((float)x_dist, abs(period_x), dir_x);
//  }
//  if (y_dist != 0) {
//    period_y = half_period * (1 / sin(theta));
//    motor_y_start((float)y_dist, abs(period_y), dir_y);
//  }
//
//  motor_x_y_enable(move_enable_x, move_enable_y);
//}

double perv_x = 0;
double perv_y = 0;

int perv_speed = 0;

void motor_move_1(double x, double y, int speed) {
  char dir_x;
  char dir_y;

  char move_enable_x;
  char move_enable_y;

  int period_x;
  int period_y;

  double x_dist = x - perv_x;
  double y_dist = y - perv_y;

  //-------------------------------------
  if (x_dist > 0) dir_x = RIGHT;
  else if (x_dist < 0) dir_x = LEFT;

  if (y_dist > 0) dir_y = UP;
  else if (y_dist < 0) dir_y = DOWN;
  //-------------------------------------
  if (x_dist == 0) move_enable_x = DISABLE;
  else move_enable_x = ENABLE;

  if (y_dist == 0) move_enable_y = DISABLE;
  else move_enable_y = ENABLE;
  //-------------------------------------

  period_x = 200;
  period_y = 200;

  if(speed == 0) speed = perv_speed;
  
  double half_period = (double)((1000000.0 / (((double)speed / 60.0) * 80)) / 2);
  //Serial.println((int)half_period);

  double theta = atan2(y_dist, x_dist);
  //Serial.println(theta,5);

  x_dist = abs(x_dist);
  y_dist = abs(y_dist);

  if (x_dist != 0) {
    period_x = half_period * (1 / cos(theta));
    motor_x_start((float)x_dist, abs(period_x), dir_x);
  }
  if (y_dist != 0) {
    period_y = half_period * (1 / sin(theta));
    motor_y_start((float)y_dist, abs(period_y), dir_y);
  }

  motor_x_y_enable(move_enable_x, move_enable_y);

  perv_x = x;
  perv_y = y;

  perv_speed = speed;
}

#define NUM_GCODE 204


//======================================================================================================================================(참조용 안됌)

//char x_value_string[10];
//char y_value_string[10];
//char e_value_string[10];
//char f_value_string[10];
//int x_value_index = 0;
//int y_value_index = 0;
//int e_value_index = 0;
//int f_value_index = 0;
//
//double xy_value[NUM_GCODE][2];
//double e_value[NUM_GCODE];
//int f_value[NUM_GCODE];

//void gcode_parsing() {
//  for (int j = 0; j < NUM_GCODE; j++) {
//    int one_gcode_length = strlen(xy_pos_string[j]);
//    //Serial.print(*(xy_pos_string[j]+i));
//
//    for (int i = 0; i < 10; i++) {
//      x_value_string[i] = 0;
//      y_value_string[i] = 0;
//      e_value_string[i] = 0;
//      f_value_string[i] = 0;
//    }
//    x_value_index = 0;
//    y_value_index = 0;
//    e_value_index = 0;
//    f_value_index = 0;
//
//    if (*(xy_pos_string[j] + 0) == 'G' && *(xy_pos_string[j] + 1) == '1') {
//      for (int i = 2; i < one_gcode_length; i++) {
//        if (*(xy_pos_string[j] + i) == 'X' && *(xy_pos_string[j] + i - 1) == ' ') {
//          while (*(xy_pos_string[j] + i + x_value_index) != ' ' || (i+x_value_index) < one_gcode_length) {
//            x_value_string[x_value_index] = *(xy_pos_string[j] + i + x_value_index);
//            x_value_index++;
//          }
//        }
//        else if (*(xy_pos_string[j] + i) == 'Y' && *(xy_pos_string[j] + i - 1) == ' ') {
//          while (*(xy_pos_string[j] + i + y_value_index) != ' ' || (i+y_value_index) < one_gcode_length) {
//            y_value_string[y_value_index] = *(xy_pos_string[j] + i + y_value_index);
//            y_value_index++;
//          }
//        }
//        else if (*(xy_pos_string[j] + i) == 'E' && *(xy_pos_string[j] + i - 1) == ' ') {
//          while (*(xy_pos_string[j] + i + e_value_index) != ' ' || (i+x_value_index) < one_gcode_length) {
//            e_value_string[e_value_index] = *(xy_pos_string[j] + i + e_value_index);
//            e_value_index++;
//          }
//        }
//        else if (*(xy_pos_string[j] + i) == 'F' && *(xy_pos_string[j] + i - 1) == ' ') {
//          while (*(xy_pos_string[j] + i + f_value_index) != ' ' || (i+f_value_index) < one_gcode_length) {
//            f_value_string[f_value_index] = *(xy_pos_string[j] + i + f_value_index);
//            f_value_index++;
//          }
//        }
//      }
//
//      xy_value[j][0] = atof(x_value_string[1]);
//      xy_value[j][1] = atof(x_value_string[1]);
//      f_value[j] = atof(x_value_string[1]);
//
//      Serial.println(x_value_string);
//      Serial.println(y_value_string);
//      Serial.println(f_value_string);
//
//      Serial.println(xy_value[j][0]);
//      Serial.println(xy_value[j][1]);
//      Serial.println(f_value[j]);
//    }
//  }
//}

//======================================================================================================================================

enum {
  X_BUFFER = 0,
  Y_BUFFER,
  E_BUFFER,
  F_BUFFER
};

char xyef_string[4][10];
int buffer_index = 0;

char which_buffer = 0;

double xyef_value[NUM_GCODE][4];

char initial_value[4] = {'X','Y','E','F'};

//void gcode_parsing() {
//  for (int j = 0; j < NUM_GCODE; j++) {
//    int one_gcode_length = strlen(xy_pos_string[j]);
//    //Serial.print(*(xy_pos_string[j]+i));
//    buffer_index = 0;
//    for (int k = 0; k < 4; k++) {
//      for (int i = 0; i < 10; i++) {
//        xyef_string[k][i] = 0;
//      }
//    }
//
//    if (*(xy_pos_string[j] + 0) == 'G' && *(xy_pos_string[j] + 1) == '1') {
//      for (int i = 2; i < one_gcode_length; i++) {
//        if (*(xy_pos_string[j] + i) == 'X') {
//          which_buffer = X_BUFFER;
//          buffer_index = 0;
//        }
//        else if (*(xy_pos_string[j] + i) == 'Y') {
//          which_buffer = Y_BUFFER;
//          buffer_index = 0;
//        }
//        else if (*(xy_pos_string[j] + i) == 'E') {
//          which_buffer = E_BUFFER;
//          buffer_index = 0;
//        }
//        else if (*(xy_pos_string[j] + i) == 'F') {
//          which_buffer = F_BUFFER;
//          buffer_index = 0;
//        }
//        if (*(xy_pos_string[j] + i) != ' ') {
//          xyef_string[which_buffer][buffer_index++] = *(xy_pos_string[j] + i);
//        }
//      }
//      for(int i=0;i<4;i++){
//        xyef_value[i] = atof(&xyef_string[i][1]);
//        Serial.println(xyef_value[i],4);
//      }
//    }
//  }
//}

void gcode_parsing() {
  for (int j = 0; j < NUM_GCODE; j++) {
    int one_gcode_length = strlen(xy_pos_string[j]);
    //Serial.print(*(xy_pos_string[j]+i));
    buffer_index = 0;
    for (int k = 0; k < 4; k++) {
      for (int i = 0; i < 10; i++) {
        xyef_string[k][i] = 0;
      }
    }

    if (*(xy_pos_string[j] + 0) == 'G' && *(xy_pos_string[j] + 1) == '1') {
      for (int i = 2; i < one_gcode_length; i++) {
        for(int m = 0;m<4;m++){
          if (*(xy_pos_string[j] + i) == initial_value[m]) {
            which_buffer = m;
            buffer_index = 0;
            break;
          }
        }
        if (*(xy_pos_string[j] + i) != ' ') {
          xyef_string[which_buffer][buffer_index++] = *(xy_pos_string[j] + i);
        }
      }
      for(int i=0;i<4;i++){
        xyef_value[j][i] = atof(&xyef_string[i][1]);
        //Serial.println(xyef_value[j][i],4);
      }
    }
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(X_STEP, OUTPUT);
  pinMode(X_DIR, OUTPUT);
  pinMode(X_ST0P, INPUT);

  pinMode(Y_STEP, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
  pinMode(Y_STOP, INPUT);

  pinMode(XY_EN, OUTPUT);

  //Timer 1 : X Motor
  TCCR1A = 0x00;
  TCCR1B = 0x0A;
  TCCR1C = 0x00;
  TCNT1 = 0x00;
  OCR1A = 400;
  TIMSK1 = 0x02;

  //Timer 3 : X Motor
  TCCR3A = 0x00;
  TCCR3B = 0x0A;
  TCCR3C = 0x00;
  TCNT3 = 0x00;
  OCR3A = 400;
  TIMSK3 = 0x02;

  //  count_distance_x = (int)(43.3 * 80);
  //  count_distance_y = (int)(25 * 80);
  //
  //  enable_motor_x = ENABLE;
  //  enable_motor_y = ENABLE;
  //
  //  helf_period_x = 200;
  //  helf_period_y = (int)(200 * 1.732);
  //
  //  OCR1A = helf_period_x * 2;
  //  OCR3A = helf_period_y * 2;
  //
  //  interrupt_stopped_x = RUNNING;
  //  interrupt_stopped_y = RUNNING;
  //
  //  motor_direction(RIGHT);
  //  motor_direction(UP);

  //motor_init();

  gcode_parsing();

//  while (1) {
//
//  }
//  motor_move(0, 0, 50, 30, 1125);
//  state = 0;
}

double gcode_array[4][2] = {{0, 0},
  {50, 0},
  {50, 50},
  {0, 50}
};

int pre_state;

void loop() {
  int gcode_index = 0;
  motor_move_1(xyef_value[gcode_index][0],xyef_value[gcode_index][1],xyef_value[gcode_index][3]);
  while (1) {
    if (interrupt_stopped_x == STOPPED) {
      timer_x_disable();
    }

    if (interrupt_stopped_y == STOPPED) {
      timer_y_disable();
    }
    //Serial.println();//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    if (interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED) {
      motor_move_1(xyef_value[gcode_index][0],xyef_value[gcode_index][1],xyef_value[gcode_index][3]);

      if (gcode_index++ == NUM_GCODE) while(1);
    }
  }

  //------------------------------------------------------------------------------------------------------------------------------

  if (state == 1) {
    if (interrupt_stopped_x == STOPPED) {
      timer_x_disable();
    }

    if (interrupt_stopped_y == STOPPED) {
      timer_y_disable();
    }

    if (interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED) {
      //      motor_y_start(25,200 * 1.732,DOWN);
      //      motor_x_y_enable(DISABLE,ENABLE);

      //delay(1000);

      //motor_move(0, 0, 0, -30, 1125);

      state = 2;
    }
  }
  else if (state == 2) {
    if (interrupt_stopped_x == STOPPED) {
      timer_x_disable();
    }

    if (interrupt_stopped_y == STOPPED) {
      timer_y_disable();
    }

    if (interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED) {
      if (interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED) {
        //         motor_x_start(43.3,200,LEFT);
        //         motor_x_y_enable(ENABLE,DISABLE);

        //motor_move(30, 0, 0, 0, 1125);
        state = 3;
      }
    }
  }
  else if (state == 3) {
    if (interrupt_stopped_x == STOPPED) {
      timer_x_disable();
    }

    if (interrupt_stopped_y == STOPPED) {
      timer_y_disable();
    }

    if (interrupt_stopped_x == STOPPED && interrupt_stopped_y == STOPPED) {
      //      motor_x_start(43.3,200,RIGHT);
      //      motor_y_start(25,200 * 1.732,UP);
      //      motor_x_y_enable(ENABLE,ENABLE);

      //motor_move(0, 0, 50, 30, 1125);

      state = 1;
    }
  }
}

SIGNAL(TIMER1_COMPA_vect)
{
  if (enable_motor_x == ENABLE) {
    if (toggle_step_x == 0) {
      toggle_step_x = 1;

      digitalWrite(X_STEP, HIGH);
    }
    else {
      toggle_step_x = 0;

      step_count_x++;
      if (step_count_x > count_distance_x) {
        digitalWrite(X_STEP, LOW);
        enable_motor_x = DISABLE;
        interrupt_stopped_x = STOPPED;
        step_count_x = 0;
      }
      else {
        digitalWrite(X_STEP, LOW);
      }
    }
  }
}

SIGNAL(TIMER3_COMPA_vect)
{
  if (enable_motor_y == ENABLE) {
    if (toggle_step_y == 0) {
      toggle_step_y = 1;

      digitalWrite(Y_STEP, HIGH);
    }
    else {
      toggle_step_y = 0;

      step_count_y++;
      if (step_count_y > count_distance_y) {
        digitalWrite(Y_STEP, LOW);
        enable_motor_y = DISABLE;
        interrupt_stopped_y = STOPPED;
        step_count_y = 0;
      }
      else {
        digitalWrite(Y_STEP, LOW);
      }
    }
  }
}