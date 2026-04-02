const int LedRed = 8;
const int LedGreen = 13;
const int LedYellow = 12;
const int trafficButton = 4;

int trafficDelay = 1000;

void setup() {
  pinMode(LedRed, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(LedYellow, OUTPUT);
  pinMode(trafficButton, INPUT_PULLUP);

  digitalWrite(LedGreen, HIGH);
  digitalWrite(LedYellow, HIGH);
}

void loop() {
  if(digitalRead(trafficButton) == LOW){
    delay(50);
    digitalWrite(LedRed, HIGH);
    delay(trafficDelay);
    digitalWrite(LedGreen, LOW);
    delay(3 * trafficDelay);
    digitalWrite(LedGreen, HIGH);
    digitalWrite(LedYellow, LOW);
    delay(trafficDelay);
    digitalWrite(LedYellow, HIGH);
    digitalWrite(LedRed, LOW);
  }
}
