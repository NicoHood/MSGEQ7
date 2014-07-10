/*
Copyright (c) 2014 NicoHood
See the readme for credit to other people.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/


#ifndef MSGEQ7_h
#define MSGEQ7_h

// needed for digitalRead() and analogRead()
#include <Arduino.h>

//================================================================================
// MSGEQ7
//================================================================================

//number of reading times for smoothing, default 5
#define MSGEQ7_READINGS 5  

// basic frequencys definitions (0-6 valid)
#define MSGEQ7_BASS 0
#define MSGEQ7_LOW 0
#define MSGEQ7_MID 3
#define MSGEQ7_HIGH 5

// channel defintion for get/peek methodes
#define MSGEQ7_LEFT_RIGHT 0
#define MSGEQ7_LEFT 1
#define MSGEQ7_RIGHT 2

class CMSGEQ7{
public:
	CMSGEQ7(void);

	// user functions to interact with the IC
	void begin(uint8_t resetPin, uint8_t strobPine,
		uint8_t analogPinLeft, uint8_t analogPinRight = 0xFF);
	void reset(void);
	void read(void);

	// function for the user to access the values, default l+r
	uint8_t get(uint8_t frequency, uint8_t channel = MSGEQ7_LEFT_RIGHT);
	uint8_t peek(uint8_t frequency, uint8_t channel = MSGEQ7_LEFT_RIGHT);

private:
	// pins to interact with the IC
	uint8_t _resetPin;
	uint8_t _strobePin;
	uint8_t _analogPinLeft;
	uint8_t _analogPinRight;

	// array of all input values
	uint8_t _left[7][MSGEQ7_READINGS];
	uint8_t _right[7][MSGEQ7_READINGS];
	uint8_t _countRead;
};

extern CMSGEQ7 MSGEQ7;

#endif

