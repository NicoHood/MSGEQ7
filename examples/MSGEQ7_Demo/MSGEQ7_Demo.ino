/*
  MSGEQ7 Demo
 Copyright (c) 2014 NicoHood.  All right reserved.
 
 Reads MSGEQ7 IC with 7 different frequencys from range 0-255
 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
 
 This is a simple Demo to show you how to access the IC and its values
 Lets a led blink to the beat of the music
 
 Some additional Information:
 *Have a look at the library itself, its not so complicated.
 But keep attention to the LOW and HIGH setting of strobe and reset
 *Use myMSGEQ7.mode=1 for direct input (default=0, smoothed value)
 *Keep in mind that the smoothed value is created from the last 5(by default) reads.
 If you analze every 20ms this means the last 20*5=100 ms time period actually is analyzed
 I found out that reading every 20ms is a good value to start.
 *You can reset the IC with myMSGEQ7.reset() but you dont need that normally
 *If you only want to use mono mode or one IC just connect the two inputs together to the MSGEQ7
 and only use the left or right value of the library. Choose the same analogpin for left/right then
 *The Output only ranges from 0-255 for saving memory and for easy use. It doesnt matter much.
 If you still want to read up to 1023 remove the divide 4 in the library and use uint16_t
 *The IC has a standard value at about 10. You should keep that in mind.
 In this case the Led always glims a bit. Have a look at the map() function to improve this.
 */

//MSGEQ7 
#include "MSGEQ7.h"
const uint8_t resetPin = 4; //white
const uint8_t strobePin = 7; //yellow
const uint8_t analogPinLeft = A0; //violett
const uint8_t analogPinRight = A1; //cyan
MSGEQ7 myMSGEQ7(resetPin, strobePin, analogPinLeft, analogPinRight);

uint8_t frequency =0;     //0 for BASS
const uint8_t ledPin = 3; //PWM Pin, green

long previousMillis = 0; // will store last time LED was updated
long interval = 20;      // interval at which to blink (milliseconds)

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() { 
  //blink without delay
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   

    //analyze
    myMSGEQ7.read();
    //let the led blink to the beat
    analogWrite(ledPin, (myMSGEQ7.left[frequency]+myMSGEQ7.right[frequency])/2);
    //analogWrite(ledPin, myMSGEQ7.left[frequency]); //uncomment for mono at Pin A0 only
  }
}

