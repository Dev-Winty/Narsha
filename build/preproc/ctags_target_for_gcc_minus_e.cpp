# 1 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\main.ino"
# 2 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\main.ino" 2
# 12 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\main.ino"
char x_stopped = 0;
char y_stopped = 0;
char motor_stop_x = 1;
char motor_stop_y = 1;
unsigned long curr_micros = 0;
unsigned long pre_micros = 0;
unsigned long curr_millis = 0;
unsigned long pre_millis = 0;
char step_toggle = 0;
char dir_toggle = 0;

unsigned int step_count_x = 0;
unsigned int step_count_y = 0;

unsigned long pre_micros_y = 0;
unsigned long pre_millis_y = 0;
char step_toggle_y = 0;
char dir_toggle_y = 0;

void setup()
{
 Serial.begin(9600);

 pinMode(21, 0x1);
 pinMode(18, 0x0);
 pinMode(15, 0x1);

 pinMode(14, 0x1);

 pinMode(23, 0x1);
 pinMode(19, 0x0);
 pinMode(22, 0x1);

 digitalWrite(21, 0x1);
 digitalWrite(23, 0x1);
 digitalWrite(14, 0x0);

 motor_stop_x = 0;
 step_count_x = 0;
}
//void loop() {
//  curr_micros = micros();
//
//  if (curr_micros - pre_micros > 200) {
//    pre_micros = curr_micros;
//    if (motor_stop_x == 0) {
//      if (step_toggle == 0) {
//        step_toggle = 1;
//        digitalWrite(X_STEP, HIGH);
//      }
//      else if (step_toggle == 1) {
//        step_toggle = 0;
//        digitalWrite(X_STEP, LOW);
//      }
//    }
//
//  }
//  int in_value = digitalRead(X_STOP);
//  if (in_value == 1) {
//    motor_stop_x = 1;
//  }

//----------------------------------------------------------

//======================================================

void loop()
{
 curr_micros = micros();
 curr_millis = millis();

 if (curr_micros - pre_micros > 200)
 {
  pre_micros = curr_micros;
  if (motor_stop_x == 0)
  {
   if (step_toggle == 0)
   {
    step_toggle = 1;
    digitalWrite(15, 0x1);
   }
   else if (step_toggle == 1)
   {
    step_toggle = 0;
    digitalWrite(15, 0x0);
    step_count_x++;
    if (step_count_x > 4000)
    {
     motor_stop_x = 1;
     step_count_x = 0;

     x_stopped = 1;
    }
   }
  }
 }
 //--------------------------------------------

 if (curr_micros - pre_micros_y > 200)
 {
  pre_micros_y = curr_micros;
  if (motor_stop_y == 0)
  {
   if (step_toggle_y == 0)
   {
    step_toggle_y = 1;
    digitalWrite(22, 0x1);
   }
   else if (step_toggle_y == 1)
   {
    step_toggle_y = 0;
    digitalWrite(22, 0x0);
    step_count_y++;
    if (step_count_y > 4000)
    {
     motor_stop_y = 1;
     step_count_y = 0;

     y_stopped = 1;
    }
   }
  } else {
   Serial.println("y스탑");
  }
 }

 //------------------------------------------

 //  if(curr_millis - pre_millis > 1000) {
 //    pre_millis = curr_millis;
 //
 //    if(dir_toggle == 0) {
 //      dir_toggle = 1;
 //      digitalWrite(X_DIR, LOW);
 //    }
 //    else if(dir_toggle == 1) {
 //      dir_toggle = 0;
 //      digitalWrite(X_DIR, HIGH);
 //    }
 //  }
 //--------------------------------

 //if(curr_millis - pre_millis_y > 2000) {
 //    pre_millis_y = curr_millis;
 //
 //    if(dir_toggle_y == 0) {
 //      dir_toggle_y = 1;
 //      digitalWrite(Y_DIR, LOW);
 //    }
 //    else if(dir_toggle_y == 1) {
 //      dir_toggle_y = 0;
 //      digitalWrite(Y_DIR, HIGH);
 //    }
 //  }

 //--------------------------------

 int in_value = digitalRead(18);
 if (in_value == 1)
 {
  motor_stop_x = 1;
 }

 //----------------------------------

 if (x_stopped == 1 && y_stopped == 0)
 {
  motor_stop_y = 0;
  step_count_y = 0;
 }
 else if (x_stopped == 1 && y_stopped == 1)
 {
  motor_stop_y = 0;
  step_count_y = 0;

  motor_stop_x = 0;
  step_count_x = 0;
 }
}
//  int in_value = digitalRead(X_STOP);
//  if(in_value == 0)
//  //for(int i = 0; i < 800; i++)
//  {
//    digitalWrite(X_STEP, HIGH);
//    delayMicroseconds(200);
//    digitalWrite(X_STEP, LOW);
//    delayMicroseconds(200);
//  }
//}
