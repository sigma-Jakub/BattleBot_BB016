#include <Adafruit_NeoPixel.h>

// Connected pins
const int FORWARD_LEFT = 6;
const int FORWARD_RIGHT = 5;
const int BACK_LEFT = 11;
const int BACK_RIGHT = 10;
const int TRIGGER_FRONT = 12;
const int ECHO_FRONT = 13;
const int TRIGGER_RIGHT = 7;
const int ECHO_RIGHT = 8;
const int NEO_PIXEL = 4;
const int SERVO_PIN = 9;
const int SENSOR_PINS[] = {A0, A1, A2, A4, A3, A5, A6, A7};
int s[8];

// Adjustable variables
const int BASE_SPEED = 230;
const int LEFT_ADJUSTED_SPEED = 238;
const int GET_BACK_DURATION = 500;
const int TURN_DURATION = 450;
const int STOP_DURATION = 100;
const int DRIVE_PAST_DURATION = 450;
const int BLINDNESS_DURATION = 600;
const int BLACK_THRESHOLD = 900;
const int PANIC_SPEED = 255;
const int START_PANIC_AFTER = 2750;
const int BACKUP_DURATION = 800;
const int OPEN_CLAWS = 1700;
const int CLOSE_CLAWS = 1050;
const int CLAWS_MOVEMENT_DURATION = 1000;
const int ACTIVATE_AFTER = 3000;
const int MOVE_TOWARDS_CONE_DURATION = 1400;
const int MOVE_INTO_MAZE_DURATION = 1800;
const int DISABLE_LINE_DETECTORS_DURATION = 10000;
const int TOTAL_PIXELS = 4;
const int ARR_SIZE = 3;
const int TURN_COLOR[ARR_SIZE] = {50, 105, 0};
const int MOVE_COLOR[ARR_SIZE] = {50, 0, 100};
const int STOP_COLOR[ARR_SIZE] = {0, 100, 0}; 

int           stage = -1;
bool          isFinished = false;
bool          isMoving = true;
bool          isPanicking = false;
int           lastFrontDistance = 0;
int           servoTargetValue;
unsigned long startTurnTime = 0;
unsigned long stopTurnTime = 0;
unsigned long afterTurnTime = 0;
unsigned long lastDistanceCheckTime = 0;
unsigned long panicStartTime = 0;
unsigned long turnExitTime = 0;
unsigned long lineDetectorDisableTime = 0;
unsigned long lastServoStatus = 0;

Adafruit_NeoPixel pixels(TOTAL_PIXELS, NEO_PIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(9600);
    pinMode(FORWARD_LEFT, OUTPUT);
    pinMode(FORWARD_RIGHT, OUTPUT);
    pinMode(BACK_LEFT, OUTPUT);
    pinMode(BACK_RIGHT, OUTPUT);
    pinMode(TRIGGER_FRONT, OUTPUT);
    pinMode(ECHO_FRONT, INPUT);
    pinMode(TRIGGER_RIGHT, OUTPUT);
    pinMode(ECHO_RIGHT, INPUT);
    pinMode(SERVO_PIN, OUTPUT);
  
    digitalWrite(FORWARD_LEFT, LOW);
    digitalWrite(FORWARD_RIGHT, LOW);
    digitalWrite(BACK_LEFT, LOW);
    digitalWrite(BACK_RIGHT, LOW);
    pixels.begin();
    pixels.clear();

    servoTargetValue = OPEN_CLAWS;
    
    for (int i = 0; i < 30; i++) {
        servoUpdate();
        delay(20);
    }

    // Waiting for movement in front
    while (true) {
        int getDistanceF = getDistanceFront();
        if (getDistanceF > 0 && getDistanceF < 24) {
            waitForCertainDuration(ACTIVATE_AFTER);
            break;
        }
    }

    // Getting into maze sequence
    moveForwardForCertainDuration(BASE_SPEED, MOVE_TOWARDS_CONE_DURATION);
    waitForCertainDuration(STOP_DURATION);

    servoTargetValue = CLOSE_CLAWS;
    unsigned long grabTime = millis();
    while (millis() - grabTime < CLAWS_MOVEMENT_DURATION) {
        servoUpdate();
    }

    turnLeftForCertainDuration(TURN_DURATION);
    waitForCertainDuration(STOP_DURATION);
    followLineForCertainDuration(MOVE_INTO_MAZE_DURATION);
    disableLineSensorsForCertainDuration(DISABLE_LINE_DETECTORS_DURATION);
}

void loop() {
    if (isFinished) {
        servoTargetValue = OPEN_CLAWS;
        stopMotors();
        servoUpdate();
        rainbowLeds();
        return;
    }

    servoUpdate();
    bool foundLine = false;
    int blackCount = 0;
    unsigned long currentTime = millis();
    int getDistanceF = getDistanceFront();
    int getDistanceR = getDistanceRight();

    // Disabling line detectors for couple of seconds after start of the maze
    if (currentTime >= lineDetectorDisableTime) {
        for (int i = 0; i < 8; i++) {
            s[i] = analogRead(SENSOR_PINS[i]);
            if (s[i] >= BLACK_THRESHOLD) {
                foundLine = true;
                blackCount++; 
            }
        } 
    }

    // If sees black square
    if (blackCount == 8) {
        isFinished = true;
        return;
    }

    // If any sensor detected black line - follow line, else move forward
    if (foundLine) {
        followLine();
    } else {
        if (stage == -1 && getDistanceF < 30 && getDistanceF > 0) {
            if (abs(getDistanceF - lastFrontDistance) > 1) {
                lastDistanceCheckTime = currentTime;
                lastFrontDistance = getDistanceF;
            } else {
                if (currentTime - lastDistanceCheckTime >= START_PANIC_AFTER) {
                    stage = 5;
                    panicStartTime = currentTime;
                }
            }
        }

        // If stuck for couple of seconds - panic (move backwards)
        if (stage == 5) {
            moveBackward(PANIC_SPEED);
          
            if (currentTime - panicStartTime >= BACKUP_DURATION) {
                stopMotors();
                lastDistanceCheckTime = currentTime;
                stage = -1;
            }
        }

        // Move forward
        if (stage == -1) {
            moveForward(BASE_SPEED, true);
      
            if (currentTime - turnExitTime >= BLINDNESS_DURATION) {
                if (getDistanceR >= 25 || getDistanceR == -1) { 
                    stopTurnTime = currentTime;
                    stage = 4;
                    return;
                }
            }
      
            if (getDistanceF > 0 && getDistanceF < 10) {
                stopMotors();
                stopTurnTime = currentTime;
                
                if (getDistanceR > 0 && getDistanceR < 15) {
                    stage = 10;
                } else {
                    stage = 1;
                }
            }
        }

        // If no wall on the right detected, drive past for a while to prevent stucking
        else if (stage == 4) {
            moveForward(BASE_SPEED, true);
            if (currentTime - stopTurnTime >= DRIVE_PAST_DURATION) {
                stopMotors();
                stopTurnTime = currentTime;
                stage = 1; 
            }
        }

        // Decide to turn right or left
        else if (stage == 1 || stage == 10) {
            if (currentTime - stopTurnTime >= STOP_DURATION) {
                if (stage == 1) {
                    turnRight(); 
                } else {
                    turnLeft(); 
                }
            
                startTurnTime = currentTime;
                stage = 2;
            }
        }

        // Stop after an action
        else if (stage == 2) {
            if (currentTime - startTurnTime >= TURN_DURATION) {
                stopMotors();
                afterTurnTime = currentTime;
                stage = 3;
            }
        }

        // Stop after a turn and then move forward
        else if (stage == 3) {
            if (currentTime - afterTurnTime >= STOP_DURATION) {
                turnExitTime = currentTime;
                stage = -1;
            }
        }
    }
}

void moveForward(int baseSpeed, boolean isMoving) {
    if (isMoving) {
        pixels.clear();
        pixels.setPixelColor(3, pixels.Color(MOVE_COLOR[0], MOVE_COLOR[1], MOVE_COLOR[2]));
        pixels.setPixelColor(2, pixels.Color(MOVE_COLOR[0], MOVE_COLOR[1], MOVE_COLOR[2]));
        pixels.show();
    
        const int TARGET_DISTANCE = 8;
        const int ADJUSTMENT_VALUE = 10;
        
        int currentDistanceR = getDistanceRight();
    
        if (currentDistanceR > 0 && currentDistanceR < 20) {
            int error = currentDistanceR - TARGET_DISTANCE;
            int correction = error * ADJUSTMENT_VALUE;
    
            int speedLeft = baseSpeed + correction;
            int speedRight = baseSpeed - correction;
            
            speedLeft = constrain(speedLeft, 0, 255);
            speedRight = constrain(speedRight, 0, 255);
            
            analogWrite(FORWARD_LEFT, speedLeft);
            analogWrite(FORWARD_RIGHT, speedRight);
        } else {
            analogWrite(FORWARD_LEFT, baseSpeed);
            analogWrite(FORWARD_RIGHT, baseSpeed);  
          }
    }
}

void moveBackward(int baseSpeed) {
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(STOP_COLOR[0], STOP_COLOR[1], STOP_COLOR[2]));
    pixels.setPixelColor(1, pixels.Color(STOP_COLOR[0], STOP_COLOR[1], STOP_COLOR[2]));
    pixels.show();
    
    analogWrite(FORWARD_LEFT, 0);
    analogWrite(FORWARD_RIGHT, 0);
    analogWrite(BACK_LEFT, baseSpeed);
    analogWrite(BACK_RIGHT, baseSpeed);
}

void stopMotors() {
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(STOP_COLOR[0], STOP_COLOR[1], STOP_COLOR[2]));
    pixels.setPixelColor(1, pixels.Color(STOP_COLOR[0], STOP_COLOR[1], STOP_COLOR[2]));
    pixels.show(); 
    
    analogWrite(BACK_LEFT, 0);
    analogWrite(BACK_RIGHT, 0);
    analogWrite(FORWARD_LEFT, 0);
    analogWrite(FORWARD_RIGHT, 0);
}

void followLine() {
    pixels.clear();
    pixels.setPixelColor(3, pixels.Color(0, 50, 100));
    pixels.setPixelColor(2, pixels.Color(0, 50, 100));
    pixels.show();
  
    const double ADJUSTMENT_VALUE = 0.2;
  
    int rightSensorsValue = s[0] + s[1] + s[2] + s[3];
    int leftSensorsValue = s[4] + s[5] + s[6] + s[7];
  
    int error = leftSensorsValue - rightSensorsValue;
    int correction = error * ADJUSTMENT_VALUE;

    double speedLeft = BASE_SPEED - correction;
    double speedRight = BASE_SPEED + correction;
    
    speedLeft = constrain(speedLeft, 0, 255);
    speedRight = constrain(speedRight, 0, 255);

    analogWrite(FORWARD_LEFT, speedLeft);
    analogWrite(FORWARD_RIGHT, speedRight);
    analogWrite(BACK_LEFT, 0);
    analogWrite(BACK_RIGHT, 0);
}

void disableLineSensorsForCertainDuration(int duration) {
    lineDetectorDisableTime = millis() + duration;
}

void readSensors() {
    for (int i = 0; i < 8; i++) {
        s[i] = analogRead(SENSOR_PINS[i]);
    }
}

void followLineForCertainDuration(int duration) {
    pixels.clear();
    pixels.setPixelColor(3, pixels.Color(0, 50, 100));
    pixels.setPixelColor(2, pixels.Color(0, 50, 100));
    pixels.show();

    unsigned long startTime = millis();
    while (millis() - startTime < duration) {
        servoUpdate();
        readSensors();
    
        const double ADJUSTMENT_VALUE = 0.2;
    
        int rightSensorsValue = s[0] + s[1] + s[2] + s[3];
        int leftSensorsValue = s[4] + s[5] + s[6] + s[7];
    
        int error = leftSensorsValue - rightSensorsValue;
        int correction = error * ADJUSTMENT_VALUE;
  
        double speedLeft = BASE_SPEED - correction;
        double speedRight = BASE_SPEED + correction;
        
        speedLeft = constrain(speedLeft, 0, 255);
        speedRight = constrain(speedRight, 0, 255);
  
        analogWrite(FORWARD_LEFT, speedLeft);
        analogWrite(FORWARD_RIGHT, speedRight);
        analogWrite(BACK_LEFT, 0);
        analogWrite(BACK_RIGHT, 0); 
    }
}

void moveForwardForCertainDuration(int baseSpeed, int duration) {
    pixels.clear();
    pixels.setPixelColor(3, pixels.Color(50, 0, 100));
    pixels.setPixelColor(2, pixels.Color(50, 0, 100));
    pixels.show();
  
    unsigned long startTime = millis();
    
    while (millis() - startTime < duration) {
        servoUpdate();
  
        analogWrite(FORWARD_LEFT, LEFT_ADJUSTED_SPEED);
        analogWrite(FORWARD_RIGHT, baseSpeed);
        analogWrite(BACK_LEFT, 0);
        analogWrite(BACK_RIGHT, 0);
    }
    
    stopMotors();
}

void turnLeftForCertainDuration(int duration) {
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(50, 100, 0));
    pixels.setPixelColor(3, pixels.Color(50, 100, 0));
    pixels.show();
  
    unsigned long startTime = millis();
    
    while (millis() - startTime < duration) {
        servoUpdate();
    
        analogWrite(FORWARD_LEFT, 0);
        analogWrite(FORWARD_RIGHT, 255);
        analogWrite(BACK_LEFT, 255);
        analogWrite(BACK_RIGHT, 0);
    }
    
    stopMotors();
}

void waitForCertainDuration(int duration) {
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(100, 0, 0));
    pixels.setPixelColor(1, pixels.Color(100, 0, 0));
    pixels.setPixelColor(2, pixels.Color(100, 0, 0));
    pixels.setPixelColor(3, pixels.Color(100, 0, 0));
    pixels.show();
    
    unsigned long startTime = millis();
    
    while (millis() - startTime < duration) {
        servoUpdate();
    }
}

void turnRight() {
    pixels.clear();
    pixels.setPixelColor(1, pixels.Color(TURN_COLOR[0], TURN_COLOR[1], TURN_COLOR[2]));
    pixels.setPixelColor(2, pixels.Color(TURN_COLOR[0], TURN_COLOR[1], TURN_COLOR[2]));
    pixels.show();
    
    analogWrite(BACK_LEFT, 0);
    analogWrite(BACK_RIGHT, 255);
    analogWrite(FORWARD_LEFT, 255);
    analogWrite(FORWARD_RIGHT, 0);
}

void turnLeft() {
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(TURN_COLOR[0], TURN_COLOR[1], TURN_COLOR[2]));
    pixels.setPixelColor(3, pixels.Color(TURN_COLOR[0], TURN_COLOR[1], TURN_COLOR[2]));
    pixels.show();
  
    analogWrite(BACK_LEFT, 255);
    analogWrite(BACK_RIGHT, 0);
    analogWrite(FORWARD_LEFT, 0);
    analogWrite(FORWARD_RIGHT, 255);
}

int getDistanceFront() {
    digitalWrite(TRIGGER_FRONT, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_FRONT, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_FRONT, LOW);

    long duration = pulseIn(ECHO_FRONT, HIGH, 10000);

    if (duration == 0) {
        return -1;
    }

    float distance = duration * 0.034 / 2;
    return (int)distance;
}

int getDistanceRight() {
    digitalWrite(TRIGGER_RIGHT, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_RIGHT, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_RIGHT, LOW);

    long duration = pulseIn(ECHO_RIGHT, HIGH, 10000);

    if (duration == 0) {
        return -1;
    }

    float distance = duration * 0.034 / 2;
    return (int)distance;
}

void servoUpdate() {
    if (millis() >= lastServoStatus) {
        lastServoStatus = millis() + 20;
        digitalWrite(SERVO_PIN, HIGH);
        delayMicroseconds(servoTargetValue);
        digitalWrite(SERVO_PIN, LOW);  
    }
}

void rainbowLeds() {
    for (int i = 0; i < 100; i++) {
        pixels.setPixelColor(0, pixels.Color(i, 100 - i, 0));
        pixels.setPixelColor(1, pixels.Color(i, 100 - i, 0));
        pixels.setPixelColor(2, pixels.Color(i, 100 - i, 0));
        pixels.setPixelColor(3, pixels.Color(i, 100 - i, 0));
        pixels.show();
        delay(10);
    }

    for (int i = 0; i < 100; i++) {
        pixels.setPixelColor(0, pixels.Color(100 - i, 0, i));
        pixels.setPixelColor(1, pixels.Color(100 - i, 0, i));
        pixels.setPixelColor(2, pixels.Color(100 - i, 0, i));
        pixels.setPixelColor(3, pixels.Color(100 - i, 0, i));
        pixels.show();
        delay(10);
    }

    for (int i = 0; i < 100; i++) {
        pixels.setPixelColor(0, pixels.Color(0, i, 100 - i));
        pixels.setPixelColor(1, pixels.Color(0, i, 100 - i));
        pixels.setPixelColor(2, pixels.Color(0, i, 100 - i));
        pixels.setPixelColor(3, pixels.Color(0, i, 100 - i));
        pixels.show();
        delay(10);
    }
}
