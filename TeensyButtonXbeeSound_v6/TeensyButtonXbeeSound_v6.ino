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
AudioPlaySdWav           playSdWav1;     //xy=230,147
AudioPlaySdWav           playSdWav2;     //xy=225,183
AudioPlaySdWav           playSdWav3;     //xy=227,221
AudioPlaySdWav           playSdWav4;     //xy=227,256
AudioPlaySdWav           playSdWav5;     //xy=228,307
AudioPlaySdWav           playSdWav6;     //xy=220,341
AudioPlaySdWav           playSdWav7;     //xy=220,375
AudioPlaySdWav           playSdWav8;     //xy=225,410
AudioPlaySdWav           playSdWav9;     //xy=212,456
AudioPlaySdWav           playSdWav10;    //xy=220,496
AudioPlaySdWav           playSdWav11;    //xy=217,534
AudioPlaySdWav           playSdWav12;    //xy=220,568
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

AudioPlaySdWav sdWavArray[] = {
  playSdWav1,
  playSdWav2,
  playSdWav3,
  playSdWav4,
  playSdWav5,
  playSdWav6,
  playSdWav7,
  playSdWav8,
  playSdWav9,
  playSdWav10,
  playSdWav11,
  playSdWav12
};
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
const int buttonDebounceMs = 30;
const int buttonCount = 3;
int buttonPin[] = {2, 3, 4};
Bounce button[buttonCount];

//
// leds
//
const int ledPin[] = {16, 17, 8};
int ledState = LOW;
unsigned long ledStartMillis[] = {0, 0, 0};
unsigned long blinkInterval = 100;

//
// mushroom selection
//
int mushroomCount = 4;
int mushroomSelectorPin[] = { 20, 21 };

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
  Serial1.begin(57600);

  NVIC_SET_PRIORITY(IRQ_UART0_STATUS, 255); // 255 = lowest priority
  NVIC_ENABLE_IRQ(IRQ_UART0_STATUS);

 NVIC_SET_PRIORITY(IRQ_UART1_STATUS, 255); // 255 = lowest priority
  NVIC_ENABLE_IRQ(IRQ_UART1_STATUS);


  setupMushroomSelection();
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
  mixer2.gain(0, 0.3);
  mixer2.gain(1, 0.3);
  mixer2.gain(2, 0.3);
  mixer2.gain(3, 0.3);
  mixer3.gain(0, 0.3);
  mixer3.gain(1, 0.3);
  mixer3.gain(2, 0.3);
  mixer3.gain(3, 0.3);
  mixer4.gain(0, 0.3);
  mixer4.gain(1, 0.3);
  mixer4.gain(2, 0.3);
  mixer4.gain(3, 0.3);
  mixer5.gain(0, 0.6);
}

void loop() {
  
  triggerBackingTrack();
  triggerXbeeActions();
  triggerButtonActions();
  triggerKeyboardActions();
  
  // turns led off after the set interval
  for (int i = 0; i < buttonCount; i++) {
  if (millis() - ledStartMillis[i] > blinkInterval) {
    digitalWrite(ledPin[i], LOW);
    }
  }
}


void triggerBackingTrack() {

  if (playSdWav13.isPlaying() == false) {
    Serial.println("Start playing 1");
    playSdWav13.play("BackTr2.wav");
  }

}

// listening for commands from other mushrooms
void triggerXbeeActions() {

  if (Serial1.available() > 0) {
    char inChar = (char)Serial1.read();
    // quick indication that button pressed from other mushroom
    for (int i = 0; i < buttonCount; i++) {
    digitalWrite(ledPin[i],HIGH);
    }
    playSound(inChar);
  }

  return;
}

// mushroom buttons pressed -> play file -> send serial command via Xbee
void triggerButtonActions() {

  for (int i = 0; i < buttonCount; i++) {
    // update debouncers
    button[i].update();
    if (button[i].risingEdge()) { // using rising edge because buttons are wired at NC
      // DO THE BUTTON PRESS ACTION
      // PLAY CLIP
      sendAndPlay(i);
      // TURN LED ON
      turnOnLeds(i);
    }
  }
  return;
}

// using serial monitor (keypresses) to simulate mushrooms
void triggerKeyboardActions() {

  if (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    log("keyboard command: ");
    log(inChar);
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

  playSound(command);
  delay(random(1000)); // randomize the clicking 
  Serial1.write(command);
  Serial1.flush();
  log("sending :" + String(command));
  
  
   //log(String(command));

  // react


  //qazdelay(1000);
  
 

  return;
}

// plays sound files, if given valid serial commands
// otherwise, generates lots of logging and erros conditions
void playSound(char command) {

  log("Got serial command :" + String(command));
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

  log("bad key:");
  log(command);
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

void playfile(char * filename) {

  // i did it!
  //log("-device:"); 
  //log(String(getMushroom()));
  log("playing: " + String(filename));
  log(" ");
  if (!playSdWav2.isPlaying()){
  playSdWav2.play(filename);
  } else {
    playSdWav3.play(filename);
  }
  return;

}

char getSoundCommand(int buttonId) {
  int sound = buttonId + getMushroom() * buttonCount; //buttonCount is a constant 3
  return soundCommand[sound];
}


int getMushroom() {
  /*
  for (int i = 0; i < mushroomCount; i++ ) {
    if (LOW == digitalRead(mushroomSelectorPin[i]))
      return i;
  }*/
  boolean D_ON = false == digitalRead(mushroomSelectorPin[0]); // HIGH LOW is switched around on board ie. LOW is ON
  boolean C_ON = false == digitalRead(mushroomSelectorPin[1]);

  return D_ON | C_ON << 1 ;

  // mushroom 0 is when all off !C_ON && !D_ON 
  // mushroom 1 is when only D_ON 
  // mushroom 2 is when only C_ON 
  // mushroom 3 is when D_ON && C_ON  
  
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
    Serial.print(getMushroom());
    Serial.print(" ");
    Serial.println(message);
  }

  return;
}

