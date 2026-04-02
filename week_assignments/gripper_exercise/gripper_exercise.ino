const int SERVO_PIN = 9;
const int LEFT_FORWARD = 6;
const int RIGHT_FORWARD = 5;
const int LEFT_BACKWARD = 11;
const int RIGHT_BACKWARD = 10;
const int OPEN_CLAWS_MS  = 1500;
const int CLOSE_CLAWS_MS = 1050;
const int ONE_SEC_DELAY = 1000;
const int PULSES_VALUE = 800;
int servoTargetValue;
long lastServoStatus = 0;

void setup() {
  pinMode(SERVO_PIN, OUTPUT);
  digitalWrite(SERVO_PIN, LOW);
  pinMode(LEFT_FORWARD, OUTPUT);
  digitalWrite(LEFT_FORWARD, LOW);
  pinMode(RIGHT_FORWARD, OUTPUT);
  digitalWrite(RIGHT_FORWARD, LOW);
  pinMode(LEFT_BACKWARD, OUTPUT);
  digitalWrite(LEFT_BACKWARD, LOW);
  pinMode(RIGHT_BACKWARD, OUTPUT);
  digitalWrite(RIGHT_BACKWARD, LOW);

  servoTargetValue = OPEN_CLAWS_MS;
  moveClaws(PULSES_VALUE);
  servoTargetValue = CLOSE_CLAWS_MS;
  moveClaws(PULSES_VALUE);
  servoTargetValue = OPEN_CLAWS_MS;
  moveClaws(PULSES_VALUE);
  delay(ONE_SEC_DELAY);
  moveForward();
  delay(ONE_SEC_DELAY);
  stopMotors();
  delay(ONE_SEC_DELAY);
  servoTargetValue = OPEN_CLAWS_MS;
  moveClaws(PULSES_VALUE);
  servoTargetValue = CLOSE_CLAWS_MS;
  moveClaws(PULSES_VALUE);
  moveForward();
  delay(ONE_SEC_DELAY);
  stopMotors();
  delay(ONE_SEC_DELAY);
  servoTargetValue = OPEN_CLAWS_MS;
  moveClaws(PULSES_VALUE);
}

void loop() {
  servoUpdate();
}

void moveClaws(long ms) {
  long start = millis();
  while(millis() - start < ms){
    servoUpdate();
  }
}

void servoUpdate() {
  //long now = millis();
  if(millis() > lastServoStatus){
    lastServoStatus = millis() + 20;
    digitalWrite(SERVO_PIN, HIGH);
    delayMicroseconds(servoTargetValue);
    digitalWrite(SERVO_PIN, LOW);
  }
}

void stopMotors(){
  digitalWrite(LEFT_FORWARD, LOW);
  digitalWrite(RIGHT_FORWARD, LOW);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
}

void moveForward(){
  digitalWrite(LEFT_FORWARD, HIGH);
  digitalWrite(RIGHT_FORWARD, HIGH);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);
}
