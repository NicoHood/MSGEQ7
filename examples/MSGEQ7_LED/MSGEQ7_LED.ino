/*
 Copyright (c) 2014 NicoHood
 See the readme for credit to other people.

 MSGEQ7 Serial example
 Output via Led

 Reads MSGEQ7 IC with 7 different frequencys from range 0-255
 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/

//MSGEQ7
#include <MSGEQ7.h>
const int pinAnalogLeft = A0;
const int pinAnalogRight = A1;
const int pinReset = A2;
const int pinStrobe = A3;

// choose a pwm pin!
const int pinLed = 3;

void setup() {
  // this will set the IC ready for reading
  MSGEQ7.begin(pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight);

  // led setup
  pinMode(pinLed, OUTPUT);
}

void loop() {
  // read without delay
  static long prevMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - prevMillis > 20) {
    prevMillis = currentMillis;

    // analyze
    MSGEQ7.read();

    // Led output
    uint8_t val = MSGEQ7.get(MSGEQ7_BASS);
    analogWrite(pinLed, val);
  }
}
