#include <Bounce2.h>

#define DEBUG true

const int buttonDebounceMs = 50;
const int buttonCount = 3;
int buttonPin[] = {2, 3, 4};
Bounce button[buttonCount];

int ledPin[] = {16, 17, 8};
int ledState = LOW;
unsigned long ledStartMillis[] = {0, 0, 0};
unsigned long blinkInterval = 100;

void setup() {
  Serial.begin(9600);
  setupButtons();
  setupLeds();

  return;
}

void loop() {
  triggerButtonActions();

   // turns led off after the set interval
  for (int i = 0; i < buttonCount; i++) {
  if (millis() - ledStartMillis[i] > blinkInterval) {
    digitalWrite(ledPin[i], LOW);
    }
  }

}


void triggerButtonActions() {
  //update debouncers
  for (int i = 0; i < buttonCount; i++) {
    button[i].update();
    if (button[i].risingEdge()) {
      turnOnLeds(i);
    }
  }
  return;
}

void setupButtons() {
  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttonPin[i], INPUT_PULLUP);
    button[i] = Bounce(buttonPin[i], buttonDebounceMs);
  }
  return;
}

void setupLeds() {
  for (int i = 0; i < buttonCount; i++) {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], LOW);
  }
  return;
}

void turnOnLeds(int buttonId){
  ledStartMillis[buttonId] = millis();
  ledState = HIGH;
  digitalWrite(ledPin[buttonId], ledState);
  log("button " + String(buttonId));
  log(" ");
  return;
}

void log (String message) {

  if (DEBUG) {
    //Serial.print(getMushroom());
    //Serial.print(" ");
    Serial.println(message);
  }

  return;
}


