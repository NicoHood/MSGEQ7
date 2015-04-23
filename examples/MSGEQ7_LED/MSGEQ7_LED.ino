/*
 Copyright (c) 2014-2015 NicoHood
 See the readme for credit to other people.

 MSGEQ7 Led example
 Output via Led (choose a PWM pin!)

 Fast setup to demonstrate functionality

 Reads MSGEQ7 IC with 7 different frequencies from range 0-255
 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/

// MSGEQ7
#include "MSGEQ7.h"
#define pinAnalog A0
#define pinReset 6
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH true

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalog> MSGEQ7;

// choose a PWM pin!
#define pinLed 11

void setup() {
  // this will set the IC ready for reading
  MSGEQ7.begin();

  // led setup
  pinMode(pinLed, OUTPUT);
}

void loop() {
  // analyze without delay every interval
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Led output
  if (newReading) {
    uint8_t input = MSGEQ7.get(MSGEQ7_BASS);

    // output PWM signal via Led to the music beat
    analogWrite(pinLed, input);
  }
}
