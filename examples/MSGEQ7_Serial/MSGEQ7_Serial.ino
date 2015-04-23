/*
 Copyright (c) 2014-2015 NicoHood
 See the readme for credit to other people.

 MSGEQ7 Serial example
 Output via Serial

 Needs no special hardware and displays raw values on the Serial monitor.

 Reads MSGEQ7 IC with 7 different frequencys from range 0-255
 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/

// MSGEQ7
#include "MSGEQ7.h"
#define pinAnalogLeft A0
#define pinAnalogRight A1
#define pinReset 6
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH false

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

void setup() {
  // this will set the IC ready for reading
  MSGEQ7.begin();

  // Serial setup with faster baud rate gives more FPS (115200)
  Serial.begin(9600);
  Serial.println("Startup");
}

void loop() {
  // analyze without delay every interval
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // calculate FPS
  uint16_t FPS = getFPS(newReading);

  // Serial raw debug output
  if (newReading) {
    // all channels together
    Serial.print(MSGEQ7.get(MSGEQ7_BASS));
    Serial.print(" ");

    // 1st channel (here: left)
    Serial.print(MSGEQ7.get(MSGEQ7_BASS, 0));
    Serial.print(" ");

    // 2nd channel (here: left)
    Serial.print(MSGEQ7.get(MSGEQ7_BASS, 1));
    Serial.print(" ");

    // overall volume of all channels
    Serial.print(MSGEQ7.getVolume());
    Serial.print(" ");

    // FPS of the reading
    Serial.print(FPS);
    Serial.print(" ");

    // visualize with characters as bars
    uint8_t input = MSGEQ7.get(MSGEQ7_BASS);
    for (uint8_t i = 0; i < (input / 4); i++)
      Serial.print('=');
    Serial.println();
  }
}

uint16_t getFPS(bool newReading) {
  // variables to count FPS and last 1 second mark
  static uint16_t prevFPS = 0;
  static uint16_t FPS = 0;
  static uint32_t m = 0;

  // increase FPS count
  if (newReading)
    FPS++;

  // if 1 second mark crossed, save new FPS
  if ((micros() - m) > 1000000) {
    prevFPS = FPS;
    FPS = 0;
    m = micros();
  }

  return prevFPS;
}
