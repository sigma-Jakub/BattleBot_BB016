const int LedRed = 8;
const int FastButtonPin = 9;
const int SlowButtonPin = 4;

int fastButtonState = 0;
int lastFastButtonState = 0;
int slowButtonState = 0;
int lastSlowButtonState = 0;

int blinkDelay = 1000;

void setup() {
  pinMode(LedRed, OUTPUT);
  pinMode(FastButtonPin, INPUT_PULLUP);
  pinMode(SlowButtonPin, INPUT_PULLUP);
}

void loop() {
  fastButtonState = digitalRead(FastButtonPin);
  slowButtonState = digitalRead(SlowButtonPin);

  if(fastButtonState == LOW && lastFastButtonState == HIGH){
      if(blinkDelay == 500) {
          blinkDelay = 1000;
        } else {
            blinkDelay = 500;
        }
    }

  if(slowButtonState == LOW && lastSlowButtonState == HIGH){
      if(blinkDelay == 2000){
          blinkDelay = 1000;
        } else {
            blinkDelay = 2000;
          }
    }

  lastFastButtonState = fastButtonState;
  lastSlowButtonState = slowButtonState;

  digitalWrite(LedRed, LOW);
  delay(blinkDelay);
  digitalWrite(LedRed, HIGH);
  delay(blinkDelay);
}
