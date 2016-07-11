// Advanced Microcontroller-based Audio Workshop
//
// https://github.com/PaulStoffregen/AudioWorkshop2015/raw/master/workshop.pdf
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 2-3: Playing Samples

// WAV files converted to code by wav2sketch

#include "AudioSampleSnare.h"        // http://www.freesound.org/people/KEVOY/sounds/82583/
#include "AudioSampleTomtom.h"       // http://www.freesound.org/people/zgump/sounds/86334/
#include "AudioSampleHihat.h"        // http://www.freesound.org/people/mhc/sounds/102790/
#include "AudioSampleKick.h"         // http://www.freesound.org/people/DWSD/sounds/171104/
#include "AudioSampleGong.h"         // http://www.freesound.org/people/juskiddink/sounds/86773/
#include "AudioSampleCashregister.h" // http://www.freesound.org/people/kiddpark/sounds/201159/

#include "AudioSampleRockslide.h"

#include <Bounce.h>


///////////////////////////////////
// copy the Design Tool code here
///////////////////////////////////

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code

AudioPlayMemory          playMem3;       //xy=237,225
AudioPlayMemory          playMem2;       //xy=244,179
AudioPlayMemory          playMem1;       //xy=249,128
AudioMixer4              mixer1;         //xy=433,178
AudioOutputI2S           i2s1;           //xy=830,277
AudioConnection          patchCord8(playMem3, 0, mixer1, 2);
AudioConnection          patchCord9(playMem2, 0, mixer1, 1);
AudioConnection          patchCord10(playMem1, 0, mixer1, 0);
AudioControlSGTL5000     sgtl5000_1;     //xy=667,550
// GUItool: end automatically generated code



// Bounce objects to read pushbuttons 
Bounce button0 = Bounce(2, 15);
Bounce button1 = Bounce(3, 15);  // 15 ms debounce time
Bounce button2 = Bounce(4, 15);


void setup() {

  NVIC_SET_PRIORITY(IRQ_UART0_STATUS, 255); // 255 = lowest priority
  NVIC_ENABLE_IRQ(IRQ_UART0_STATUS);

  Serial1.begin(9600);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  mixer1.gain(0, 0.3);
  mixer1.gain(1, 0.3);
  mixer1.gain(2, 0.3);

}

void loop() {
  // Update all the button objects
  button0.update();
  button1.update();
  button2.update();

  if (button0.fallingEdge()) {
    playMem1.play(AudioSampleSnare);
    Serial1.println("hello");
    //delay(500);
  }

  
  if (button1.fallingEdge()) {
    //delay(500);
    playMem2.play(AudioSampleKick);
    Serial1.println("hello");
   // delay(500);
  }

  
  if (button2.fallingEdge()) {
    playMem3.play(AudioSampleHihat);  
    Serial1.println("hello");
  }

}

