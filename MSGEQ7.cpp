/*
  MSGEQ7.cpp - library - description
 Copyright (c) 2014 NicoHood.  All right reserved.
 
 Reads MSGEQ7 IC with 7 different frequencys from range 0-255
 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
 */

#include "MSGEQ7.h"

MSGEQ7::MSGEQ7(uint8_t resetPin, uint8_t strobePin, uint8_t analogPinLeft, uint8_t analogPinRight){ //constructor
  // initialize this instance's variables
  _resetPin=resetPin;
  _strobePin=strobePin;
  _analogPinLeft=analogPinLeft;
  _analogPinRight=analogPinRight;
  mode=0; //default mode

  // do whatever is required to initialize the library
  pinMode(_resetPin, OUTPUT);
  pinMode(_strobePin, OUTPUT);
  pinMode(_analogPinLeft, INPUT);
  pinMode(_analogPinRight, INPUT);
  reset();
}

void MSGEQ7::reset(){
  digitalWrite(_strobePin, LOW); //only this setting works properly
  digitalWrite(_resetPin, HIGH);
  digitalWrite(_resetPin, LOW); 
}

void MSGEQ7::read(){//write external variables
  //digitalWrite(_resetPin, HIGH); //resets the IC, optional
  //digitalWrite(_resetPin, LOW);
  for (int i = 0; i < 7; i++){  //63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
    digitalWrite(_strobePin, HIGH); //Prepare for next value
    digitalWrite(_strobePin, LOW); //Next value for the IC
    delayMicroseconds(36); // to allow the output to settle
    left[i] = _left[i][_countRead]= analogRead(_analogPinLeft) /4;  //read left pin,  range 0-255
    right[i]=_right[i][_countRead]= analogRead(_analogPinRight)/4;  //read right pin, range 0-255

    if (mode==0){ //smoothing
      uint16_t avleft=0, avright=0; //resets average values
      for (int j=0; j<TOTALREADINGS;j++){//writing smoothed value from average
        avleft+=_left[i][j];
        avright+=_right[i][j];    
      }
      left[i]=avleft/TOTALREADINGS;
      right[i]=avright/TOTALREADINGS;
    }
  }

  _countRead++; //add counter+1 for next reading value
  if (_countRead==TOTALREADINGS) _countRead=0; //restart counter
}


