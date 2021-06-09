# 1 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
# 2 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 2







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
        digitalWrite(21, 0x1);
    } else if (dir == RIGHT) {
        digitalWrite(21, 0x0);
    } else if (dir == UP) {
        digitalWrite(23, 0x0);
    } else if (dir == DOWN) {
        digitalWrite(23, 0x1);
    }
}

void timerXDisable() {
    
# 59 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x6F)) 
# 59 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x00;
}

void timerXEnable() {
    
# 63 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint16_t *)(0x94)) 
# 63 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
         = 0;
    
# 64 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x6F)) 
# 64 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x02;
}

void timerYDisable() {
    
# 68 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x71)) 
# 68 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x00;
}

void timerYEnable() {
    
# 72 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint16_t *)(0x94)) 
# 72 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
         = 0;
    
# 73 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x71)) 
# 73 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x02;
}

void motorXStart(float distance, int stepPeriod, char direction) {

    countDistanceX = (int)(distance * 80);

    halfPeriodX = stepPeriod;

    
# 82 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint16_t *)(0x88)) 
# 82 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
         = halfPeriodX * 2;

    inturruptStoppedX = RUNNING;

    motorDirection(direction);
}

void motorYStart(float distance, int stepPeriod, char direction) {

    countDistanceY = (int)(distance * 80);

    halfPeriodY = stepPeriod;

    
# 95 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint16_t *)(0x98)) 
# 95 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
         = halfPeriodY * 2;

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

void motorMove(double startX, double startY, double endX, double endY) {
    float xDist = endX - startX;
    float yDist = endY - startY;

    char dirX;
    char dirY;

    double theta = atan2(yDist, xDist);

    int periodX = 200;
    int periodY = (int)(200.0 * (xDist / yDist));

    char moveEnableX;
    char moveEnableY;

    // Serial.println(theta);

    if (xDist > 0) {
        dirX = RIGHT;
    } else if (xDist < 0) {
        dirX = LEFT;
    }

    if (xDist == 0) {
        moveEnableX = DISABLE;
    } else {
        moveEnableX = ENABLE;
    }

    if (yDist == 0) {
        moveEnableY = DISABLE;
    } else if (yDist < 0) {
        moveEnableY = ENABLE;
    }

    if (yDist > 0) {
        dirY = UP;
    } else {
        dirY = DOWN;
    }

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

    pinMode(21, 0x1);
    pinMode(15, 0x1);

    pinMode(14, 0x1);

    pinMode(23, 0x1);
    pinMode(22, 0x1);

    digitalWrite(14, 0x0);

    
# 188 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x80)) 
# 188 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x00;
    
# 189 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x81)) 
# 189 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x0A;
    
# 190 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x82)) 
# 190 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x00;
    
# 191 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint16_t *)(0x84)) 
# 191 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
         = 0x00;
    
# 192 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x6F)) 
# 192 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x02;

    
# 194 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x90)) 
# 194 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x00;
    
# 195 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x91)) 
# 195 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x0A;
    
# 196 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x92)) 
# 196 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x00;
    
# 197 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint16_t *)(0x94)) 
# 197 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
         = 0x00;
    
# 198 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
   (*(volatile uint8_t *)(0x71)) 
# 198 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
          = 0x02;

    // motorInit();

    state = 1;
    motorMove(50, 30, 50, 0);

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

            // motorYStart(25, 200 * 1.732, DOWN);
            // motorXYEnable(DISABLE, ENABLE);

            motorMove(0, 0, 0, 30);
            state = 2;
        }
    } else if (state == 2) {

        if (inturruptStoppedX == STOPPED) {
            timerXDisable();
        }

        if (inturruptStoppedX == STOPPED) {
            timerXDisable();
        }
        if (inturruptStoppedX == STOPPED && inturruptStoppedY == STOPPED) {

            // motorXStart(43.3, 200, LEFT);
            // motorXYEnable(ENABLE, DISABLE);

            // motorMove(30, 0, 0, 0);
            state = 3;
        }
    } else if (state == 3) {
        // state = 4;

        // inturruptStoppedX = STOPPED;
        // inturruptStoppedY = STOPPED;

        // enableMotorX = DISABLE;
        // enableMotorY = DISABLE;

        // timerXDisable();
        // timerYDisable();
        if (inturruptStoppedX == STOPPED) {
            timerXDisable();
        }

        if (inturruptStoppedX == STOPPED) {
            timerXDisable();
        }
        if (inturruptStoppedX == STOPPED && inturruptStoppedY == STOPPED) {

            // motorXStart(43.3, 200, RIGHT);
            // motorYStart(25, 200 * 1.732, UP);
            // motorXYEnable(ENABLE, ENABLE);
            motorMove(0, 0, 50, 30);
            state = 1;
        }
    }

}


# 274 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
extern "C" void __vector_13 /* Timer/Counter1 Compare Match A */(void) __attribute__ ((signal, used, externally_visible)); void __vector_13 /* Timer/Counter1 Compare Match A */ (void)

# 275 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
{

    if (enableMotorX == ENABLE) {
        if (toggleStepX == 0) {

            digitalWrite(15, 0x1);
            toggleStepX = 1;
        } else {
            toggleStepX = 0;

            stepCountX++;
            if (stepCountX > countDistanceX) {
                digitalWrite(15, 0x0);
                enableMotorX = DISABLE;
                inturruptStoppedX = STOPPED;
                stepCountX = 0;
            } else {
                digitalWrite(15, 0x0);
            }
        }
    }
}


# 298 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino" 3
extern "C" void __vector_32 /* Timer/Counter3 Compare Match A */(void) __attribute__ ((signal, used, externally_visible)); void __vector_32 /* Timer/Counter3 Compare Match A */ (void)

# 299 "c:\\Users\\USER\\Documents\\PlatformIO\\Projects\\Narsha\\src\\main\\motorTT\\motorTT.ino"
{

    if (enableMotorY == ENABLE) {
        if (toggleStepY == 0) {

            digitalWrite(22, 0x1);
            toggleStepY = 1;
        } else {
            toggleStepY = 0;

            stepCountY++;
            if (stepCountY > countDistanceY) {
                digitalWrite(22, 0x0);
                enableMotorY = DISABLE;
                inturruptStoppedY = STOPPED;
                stepCountY = 0;
            } else {
                digitalWrite(22, 0x0);
            }
        }
    }
}
