const int LEFT_FORWARD = 6;
const int RIGHT_FORWARD = 5;
const int LEFT_BACKWARD = 11;
const int RIGHT_BACKWARD = 10;

const int LEFT_SPEED = 255;
const int RIGHT_SPEED = 245;

int oneMeterDelay = 4000;

void setup() {
  Serial.begin(9600);
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  
  moveForward();
  delay(oneMeterDelay);
  stopMotors();
  moveBackward();
  delay(oneMeterDelay);
  stopMotors();
  turnLeft();
  delay(700);
  stopMotors();
  turnRight();
  delay(650);
  stopMotors();
}

void loop(){}

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

void turnLeft(){
  analogWrite(RIGHT_FORWARD, RIGHT_SPEED);
  digitalWrite(RIGHT_BACKWARD, LOW);
}

void turnRight(){
  analogWrite(LEFT_FORWARD, LEFT_SPEED);
  digitalWrite(LEFT_BACKWARD, LOW);
}

void stopMotors(){
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
  delay(1000);
}
