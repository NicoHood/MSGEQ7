/*
  MSGEQ7.h - library - description
 Copyright (c) 2014 NicoHood.  All right reserved.
 
 Reads MSGEQ7 IC with 7 different frequencys from range 0-255
 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
 */

// ensure this library description is only included once
#ifndef MSGEQ7_h
#define MSGEQ7_h

#include "Arduino.h"

#define TOTALREADINGS 5  //number of reading times for smoothing, default 5

class MSGEQ7{
public:
  MSGEQ7(uint8_t, uint8_t, uint8_t ,uint8_t); //constructor

  uint8_t left[7];  //left  input 0-255
  uint8_t right[7]; //right input 0-255
  uint8_t mode;     //0=smooth, 1=direct

  void reset();
  void read();

private:
  uint8_t _resetPin;
  uint8_t _strobePin;
  uint8_t _analogPinLeft;
  uint8_t _analogPinRight;

  uint8_t _left[7][TOTALREADINGS]; //array of all input values
  uint8_t _right[7][TOTALREADINGS];
  uint8_t _countRead;
};

#endif

