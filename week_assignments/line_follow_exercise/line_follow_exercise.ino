const int FORWARD_LEFT = 6;
const int FORWARD_RIGHT = 5;
const int BACK_LEFT = 11;
const int BACK_RIGHT = 10;
const int ECHO_PIN = 7;
const int TRIGGER_PIN = 12;

const int SENSOR_PINS[] = {A0, A1, A2, A4, A3, A5, A6, A7};
int s[8];

const int threshold = 480;
const int sensorStopValue = 900;
int baseSpeed = 215;

void setup() {
  Serial.begin(9600);
  pinMode(FORWARD_LEFT, OUTPUT);
  pinMode(FORWARD_RIGHT, OUTPUT);
  pinMode(BACK_LEFT, OUTPUT);
  pinMode(BACK_RIGHT, OUTPUT);
  pinMode(ECHO_PIN, INPUT_PULLUP);
  pinMode(TRIGGER_PIN, OUTPUT);
}

void loop() {  
  for (int i = 0; i < 8; i++) {
    s[i] = analogRead(SENSOR_PINS[i]);
  }

  Serial.println(s[0]);
  Serial.println(s[1]);
  Serial.println(s[2]);
  Serial.println(s[3]);
  Serial.println(s[4]);
  Serial.println(s[5]);
  Serial.println(s[6]);
  Serial.println(s[7]);
  Serial.println("__________");
  delay(2000);
//  black = 1000
// white = 200 - 450

    if(s[0] > sensorStopValue && s[1] > sensorStopValue && s[2] > sensorStopValue && s[3] > sensorStopValue && 
       s[4] > sensorStopValue && s[5] > sensorStopValue && s[6] > sensorStopValue && s[7] > sensorStopValue){
      stopMotors();
    }
    else if((s[3] > threshold && s[4] > threshold) || 
       (s[1] > threshold && s[2] > threshold && s[3] > threshold && s[4] > threshold) || 
       (s[3] > threshold && s[4] > threshold && s[5] > threshold && s[6] > threshold && s[7] > threshold)) {
      moveMotors(baseSpeed, baseSpeed);
    }
    else if (s[2] > threshold && s[3] > threshold) {
      moveMotors(baseSpeed + 20, baseSpeed - 20);
    } 
    else if (s[4] > threshold && s[5] > threshold) {
      moveMotors(baseSpeed - 20, baseSpeed + 20);
    }
    else if (s[1] > threshold){
      moveMotors(baseSpeed + 40, 0); 
    }
    else if (s[6] > threshold){
      moveMotors(0, baseSpeed + 40); 
    }
    else if (s[1] > threshold && s[0] > threshold){
      moveMotors(baseSpeed + 55, 0); 
    }
    else if (s[6] > threshold && s[7] > threshold){
      moveMotors(0, baseSpeed + 55); 
    }
  }

void moveMotors(int left, int right) {
  analogWrite(FORWARD_LEFT, left);
  analogWrite(BACK_LEFT, 0);
  analogWrite(FORWARD_RIGHT, right);
  analogWrite(BACK_RIGHT, 0);
}

void stopMotors(){
  analogWrite(BACK_LEFT, 0);
  analogWrite(BACK_RIGHT, 0);
  analogWrite(FORWARD_LEFT, 0);
  analogWrite(FORWARD_RIGHT, 0);
}
