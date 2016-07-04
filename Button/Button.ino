#include <Bounce2.h>

const int buttonDebounceMs = 50;
const int buttonCount = 3;
int buttonPin[] = {2, 3, 4};
Bounce button[buttonCount];

int ledPin[] = {16, 17, 8};

void setup() {
  setupButtons();
  setupLeds();

  return;
}

void loop() {
  triggerButtonActions();
}

void triggerButtonActions() {
  //update debouncers
  for (int i = 0; i < buttonCount; i++) {
    button[i].update();
    if (button[i].risingEdge()) {
      digitalWrite(ledPin[i], HIGH);
      Serial.print("button ");
      Serial.println(i);
      delay(100);
      digitalWrite(ledPin[i], LOW);
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



