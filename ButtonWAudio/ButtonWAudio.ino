#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav2;     //xy=225,183
AudioPlaySdWav           playSdWav3;     //xy=227,221
AudioPlaySdWav           playSdWav1;     //xy=230,147
AudioPlaySdWav           playSdWav4;     //xy=254,260
AudioMixer4              mixer1;         //xy=433,178
AudioOutputI2S           i2s1;           //xy=616,172
AudioConnection          patchCord1(playSdWav2, 1, mixer1, 1);
AudioConnection          patchCord2(playSdWav3, 0, mixer1, 2);
AudioConnection          patchCord3(playSdWav1, 1, mixer1, 0);
AudioConnection          patchCord4(playSdWav4, 0, mixer1, 3);
AudioConnection          patchCord5(mixer1, 0, i2s1, 0);
AudioConnection          patchCord6(mixer1, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=667,438
// GUItool: end automatically generated code


AudioPlaySdWav sdWavArray[] = {
  playSdWav1,
  playSdWav2,
  playSdWav3,
  playSdWav4
};

#include <Bounce2.h>

#define DEBUG true

int maxSounds = 3;
int errorSound = 3;
char* soundFileName[] =
{
  "Forest.wav",
  "River.wav",
  "RockSl.wav",
  "error.wav"
};

int soundCommandCount = 3;
char soundCommand[] = {
  'q', 'a', 'z'
};


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
  setupAudio();
  setupButtons();
  setupLeds();

  return;
}

void setupAudio() {

  // said this is needed for simultaneous cd card access
  //AudioNoInterrupts();

  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);

  SPI.setMOSI(7);
  SPI.setSCK(14);
  if (!(SD.begin(10))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  mixer1.gain(0, 0.3);
  mixer1.gain(1, 0.3);
  mixer1.gain(2, 0.3);
  mixer1.gain(3, 0.3);
}


void loop() {
  triggerBackingTrack();
  triggerButtonActions();

   // turns led off after the set interval
  for (int i = 0; i < buttonCount; i++) {
  if (millis() - ledStartMillis[i] > blinkInterval) {
    digitalWrite(ledPin[i], LOW);
    }
  }

}


void triggerBackingTrack() {

  if (playSdWav4.isPlaying() == false) {
    Serial.println("Start playing 1");
    playSdWav4.play("BackTr2.wav");
  }
}

void triggerButtonActions() {
  //update debouncers
  for (int i = 0; i < buttonCount; i++) {
    button[i].update();
    if (button[i].risingEdge()) {
      sendAndPlay(i);
      turnOnLeds(i);
    }
  }
  return;
}

void sendAndPlay(int buttonNumber) {

  char command = soundCommand[buttonNumber]; // maps button press to the mushroom
  playSound(command);
  return;
}

void playSound(char command) {
  log("Got serial command :" + String(command));
  char* soundFileName = parseSoundCommand(command);
  log("Got file : " + String(soundFileName));
  playfile(soundFileName);
  return;
}

char* parseSoundCommand(char command) {
  int sound = getSound(command);
  if (-1 == sound) {
    return soundFileName[errorSound];
  }
  return soundFileName[sound];
}

int getSound(char command) {
  for (int i = 0; i < soundCommandCount; i++) {
    log("soundCommand" + String(command));
    if (soundCommand[i] == command) {
      return i;
    }
  }
  return -1;

}

void playfile(char * filename) {
  playSdWav1.play(filename);
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


