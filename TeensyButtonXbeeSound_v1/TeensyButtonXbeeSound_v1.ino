// Symphonic Picnic for PlayTimesSquared
// Project by terry li, sumanth srinivasan, zach dorsett and adelle lin
// code created by adelle lin and matt pinner
//
// Using:
//  - Audio System Design Tool for Teensy : http://www.pjrc.com/teensy/td_libs_Audio.html
//  - OshPark boards : https://oshpark.com/shared_projects/lgvLRQTg


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioPlaySdWav           playSdWav9;     //xy=212,456
AudioPlaySdWav           playSdWav11;    //xy=217,534
AudioPlaySdWav           playSdWav6;     //xy=220,341
AudioPlaySdWav           playSdWav7;     //xy=220,375
AudioPlaySdWav           playSdWav10;    //xy=220,496
AudioPlaySdWav           playSdWav12;    //xy=220,568
AudioPlaySdWav           playSdWav2;     //xy=225,183
AudioPlaySdWav           playSdWav8;     //xy=225,410
AudioPlaySdWav           playSdWav3;     //xy=227,221
AudioPlaySdWav           playSdWav4;     //xy=227,256
AudioPlaySdWav           playSdWav5;     //xy=228,307
AudioPlaySdWav           playSdWav1;     //xy=230,147
AudioPlaySdWav           playSdWav13;    //xy=279,619
AudioMixer4              mixer1;         //xy=433,178
AudioMixer4              mixer2;         //xy=439,327
AudioMixer4              mixer4;         //xy=455,478
AudioMixer4              mixer5;         //xy=486,601
AudioMixer4              mixer3;         //xy=651,392
AudioOutputI2S           i2s1;           //xy=830,277
AudioConnection          patchCord1(playSdWav9, 0, mixer4, 0);
AudioConnection          patchCord2(playSdWav11, 0, mixer4, 2);
AudioConnection          patchCord3(playSdWav6, 0, mixer2, 1);
AudioConnection          patchCord4(playSdWav7, 0, mixer2, 2);
AudioConnection          patchCord5(playSdWav10, 0, mixer4, 1);
AudioConnection          patchCord6(playSdWav12, 0, mixer4, 3);
AudioConnection          patchCord7(playSdWav2, 1, mixer1, 1);
AudioConnection          patchCord8(playSdWav8, 0, mixer2, 3);
AudioConnection          patchCord9(playSdWav3, 0, mixer1, 2);
AudioConnection          patchCord10(playSdWav4, 0, mixer1, 3);
AudioConnection          patchCord11(playSdWav5, 0, mixer2, 0);
AudioConnection          patchCord12(playSdWav1, 1, mixer1, 0);
AudioConnection          patchCord13(playSdWav13, 0, mixer5, 0);
AudioConnection          patchCord14(mixer1, 0, mixer3, 0);
AudioConnection          patchCord15(mixer2, 0, mixer3, 1);
AudioConnection          patchCord16(mixer4, 0, mixer3, 2);
AudioConnection          patchCord17(mixer5, 0, mixer3, 3);
AudioConnection          patchCord18(mixer3, 0, i2s1, 0);
AudioConnection          patchCord19(mixer3, 0, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=667,550
// GUItool: end automatically generated code

// button debounce lib
#include <Bounce2.h>

#define DEBUG true

int maxSounds = 12;
int errorSound = 12;
char* soundFileName[] =
{
  "Forest.wav",
  "River.wav",
  "RockSl.wav",
  "EverG.wav",
  "EarthQ.wav",
  "Lava.wav",
  "RockMove.wav",
  "FireR.wav",
  "Birds.wav",
  "Lava.wav",
  "ThunderS.wav",
  "Swamp.wav",
  "error.wav"
};


//
// buttons
//
const int buttonDebounceMs = 50;
const int buttonCount = 3;
int buttonPin[] = {2, 3, 4};
Bounce button[buttonCount];

//
// mushroom selection
//
int mushroomCount = 4;
int mushroomSelectorPin[] = { 20, 21, A10, A11 };


//
// sound mapping - first "row" is mushroom 0
//
int soundCommandCount = 12;
char soundCommand[] = {
  'q', 'a', 'z',
  'w', 's', 'x',
  'e', 'd', 'c',
  'r', 'f', 'v'
};


void setup() {
  // debug / kwyboard // usb
  setupAudio();
  
  Serial.begin(9600);

  // xbee (pin 0 and 1)
  Serial1.begin(9600);
  
  setupMushroomSelection();
  setupButtons();
  
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
  mixer1.gain(0, 0.1);
  mixer1.gain(1, 0.3);
  mixer1.gain(2, 0.3);
  mixer1.gain(3, 0.1);
  mixer2.gain(0, 0.3);
  mixer2.gain(1, 0.3);
  mixer2.gain(2, 0.3);
  mixer2.gain(3, 0.1);
  mixer3.gain(0, 0.3);
  mixer3.gain(1, 0.3);
  mixer3.gain(2, 0.3);
  mixer3.gain(3, 0.1);
  mixer4.gain(0, 0.3);
  mixer4.gain(1, 0.3);
  mixer4.gain(2, 0.3);
  mixer4.gain(3, 0.1);
  mixer5.gain(0, 0.5);
}

void loop() {
  triggerXbeeActions();
  triggerButtonActions();
  triggerKeyboardActions();
}


// listening for commands from other mushrooms 
void triggerXbeeActions() {

  if (Serial1.available() > 0) {
    char inChar = (char)Serial1.read();
    playSound(inChar);
  }

  return;
}


// mushroom buttons pressed -> play file -> send serial command via Xbee
void triggerButtonActions() {
  // update debouncers
  for (int i = 0; i < buttonCount; i++ ) {
    button[i].update();
  }

  // update debouncers
  for (int i = 0; i < buttonCount; i++ ) {
    if (button[i].fallingEdge()) {
      // DO THE BUTTON PRESS ACTION
      sendAndPlay(i);
    }

  }

  return;
}


// using serial monitor (keypresses) to simulate mushrooms 
void triggerKeyboardActions() {

  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    log("keyboard command: " + inChar);
    sendAndPlay(getSound(inChar));  
  }

  return;
}


// map button number on mushroom to serial character
void sendAndPlay(int buttonNumber) {

  // make command string
  char command = getSoundCommand(buttonNumber);
  sendAndPlay(command);
  return;

}

// sending serial command to other Xbees + play file
void sendAndPlay(char command) {

  Serial1.print(command);
  Serial1.flush();

  log("sending :" + command);

  // react
  playSound(command);

  return;
  
}

// plays sound files, if given valid serial commands
// otherwise, generates lots of logging and erros conditions
void playSound(char command) {

  log("Got serial command");
  char* soundFileName = parseSoundCommand(command);
  log("Got file : " + String(soundFileName));
  playfile(soundFileName);

  return;

}

// parse command message
int getSound(char command) {
  for (int i = 0; i < soundCommandCount; i++) {
    if (soundCommand[i] == command) {
      return i;
    }
  }

  log("bad key:" + command);
  return -1;
}


// return sound filename for give command
char* parseSoundCommand(char command) {
  log("parseSoundCommand");
  int sound = getSound(command);
  if (sound == -1) {
    //if error
    return soundFileName[errorSound];
  }

  return soundFileName[sound];
}


void log (String message) {

  if (DEBUG) {
    Serial.println(getMushroom() + " " + message);
  }

  return;
}


// plays given filename
void playfile(char * filename) {

  // i did it!
  log("-device:" + String(getMushroom()) + " playing: " + String(filename));
  playSdWav1.play(filename);

  return;

}



char getSoundCommand(int buttonId) {
  int sound = buttonId + getMushroom() * buttonCount;
  return soundCommand[sound];
}


int getMushroom() {
  for (int i = 0; i < mushroomCount; i++ ) {
    if (LOW == digitalRead(mushroomSelectorPin[i]))
      return i;
  }

  log("No Mushroom Selected!");
  return -1;
}

void setupMushroomSelection() {
  for (int i = 0; i < mushroomCount; i++ ) {
    pinMode((mushroomSelectorPin[i]), INPUT_PULLUP);
  }
  return;
}

void setupButtons() {
  
  for (int i = 0; i < buttonCount; i++ ) {
    pinMode(buttonPin[i], INPUT_PULLUP);
    button[i] = Bounce(buttonPin[i], buttonDebounceMs); 
  }

  return;
}




