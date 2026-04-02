const int LEFT_FORWARD = 6;
const int RIGHT_FORWARD = 5;
const int LEFT_BACKWARD = 11;
const int RIGHT_BACKWARD = 10;

const int TRIGGER_PIN = 12;
const int ECHO_PIN = 13;

const int LEFT_SPEED = 220;
const int RIGHT_SPEED = 200;

int halfSecondDelay = 500;
int quarterSecondDelay = 250;
int almostOneSecondDelay = 825;
int oneSecondDelay = 1000;
int twoSecondDelay = 2000;

void setup(){
  Serial.begin(9600);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  delay(3000);
}

void loop(){
  delay(50);
  int distance = getDistance();
  
  if (distance > 0 && distance < 12) {
    squareSequence();
    stopMotors();
    moveForward();
  } 
  else {
    moveForward();
  }

}

void moveForward(){
  analogWrite(LEFT_FORWARD, LEFT_SPEED);
  analogWrite(RIGHT_FORWARD, RIGHT_SPEED);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
}

void moveBackward(){
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  analogWrite(LEFT_BACKWARD, LEFT_SPEED);
  analogWrite(RIGHT_BACKWARD, RIGHT_SPEED);
}

void turnRight(){
  analogWrite(LEFT_FORWARD, LEFT_SPEED);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  analogWrite(RIGHT_BACKWARD, RIGHT_SPEED);
}

void turnLeft(){
  digitalWrite(LEFT_FORWARD, LOW);
  analogWrite(LEFT_BACKWARD, LEFT_SPEED);
  analogWrite(RIGHT_FORWARD, RIGHT_SPEED);
  digitalWrite(RIGHT_BACKWARD, LOW);
}

void stopMotors(){
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
  delay(halfSecondDelay);
}

void squareSequence(){
  stopMotors();
  moveBackward();
  delay(quarterSecondDelay);
  stopMotors();
  turnRight();
  delay(halfSecondDelay);
  stopMotors();
  moveForward();
  delay(oneSecondDelay);
  stopMotors();
  turnLeft();
  delay(halfSecondDelay);
  stopMotors();
  moveForward();
  delay(twoSecondDelay);
  stopMotors();
  turnLeft();
  delay(halfSecondDelay);
  stopMotors();
  moveForward();
  delay(oneSecondDelay);
  stopMotors();
  turnRight();
  delay(halfSecondDelay);
  stopMotors();
}

int getDistance(){
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 10000);

  if (duration == 0) {
    return -1;
  }

  float distance = duration * 0.034 / 2;
  return (int)distance;
}
