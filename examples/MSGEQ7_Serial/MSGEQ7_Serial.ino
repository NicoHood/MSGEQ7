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
  // This will set the IC ready for reading
  MSGEQ7.begin();

  // Serial setup with faster baud rate gives more FPS (115200)
  Serial.begin(115200);
  Serial.println(F("Startup"));
}

void loop() {
  // Analyze without delay every interval
  bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

  // Calculate FPS
  uint16_t FPS = getFPS(newReading);

  // Serial raw debug output
  if (newReading)
    serialBars(FPS);
}

void serialBars(uint16_t FPS) {
  // Visualize the average bass of both channels
  uint8_t input = MSGEQ7.get(MSGEQ7_BASS);

  // Reduce noise
  input = mapNoise(input);

  // Save the difference between the last beat
  static uint8_t lastInput = 0;
  int delta = input - lastInput;
  lastInput = input;

  // All channels together
  Serial.print(input);
  Serial.print(F(" In\t"));

  // Difference between last measurement
  Serial.print(delta);
  Serial.print(F(" D\t"));

  // 1st channel (here: left)
  Serial.print(MSGEQ7.get(MSGEQ7_BASS, 0));
  Serial.print(F(" R\t"));

  // 2nd channel (here: left)
  Serial.print(MSGEQ7.get(MSGEQ7_BASS, 1));
  Serial.print(F(" L\t"));

  // overall volume of all channels
  Serial.print(MSGEQ7.getVolume());
  Serial.print(F(" Vol\t"));

  // FPS of the reading
  Serial.print(FPS);
  Serial.print(F(" FPS \t"));

  // Highlight high pitches
  char c = '=';
  if (delta >= 20)
    c = '#';

  // Visualize with characters as bars
  for (uint8_t i = 0; i < (input / 4); i++)
  {
    if (i == 192 / 4 - 1)
      Serial.print('+');
    else if (i == 128 / 4 - 1)
      Serial.print('*');
    else if (i == 64 / 4 - 1)
      Serial.print('X');
    else
      Serial.print(c);
  }
  Serial.println();
}

uint16_t getFPS(bool newReading) {
  // Variables to count FPS and last 1 second mark
  static uint16_t prevFPS = 0;
  static uint16_t FPS = 0;
  static uint32_t m = 0;

  // Increase FPS count
  if (newReading)
    FPS++;

  // If 1 second mark crossed, save new FPS
  if ((micros() - m) > 1000000) {
    prevFPS = FPS;
    FPS = 0;
    m = micros();
  }

  return prevFPS;
}
