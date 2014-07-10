/*
 Copyright (c) 2014 NicoHood
 See the readme for credit to other people.

 MSGEQ7 Serial example
 Output via Serial

 Reads MSGEQ7 IC with 7 different frequencys from range 0-255
 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/

//MSGEQ7
#include <MSGEQ7.h>
const int pinAnalog = A0;
const int pinReset = A2;
const int pinStrobe = A3;

void setup() {
  // this will set the IC ready for reading
  // note that we only use one input here
  MSGEQ7.begin(pinReset, pinStrobe, pinAnalogLeft);

  // Serial setup with fastest baud
  Serial.begin(115200);
  Serial.println("Startup");
}

void loop() {
  // read without delay
  static long prevMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - prevMillis > 20) {
    prevMillis = currentMillis;

    // analyze
    MSGEQ7.read();

    // Serial output
    uint8_t val = MSGEQ7.get(MSGEQ7_BASS);
    for (int i = 0; i < val / 4; i++)
      Serial.print("=");
    Serial.println();
  }
}
