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

#include "MSGEQ7.h"

//================================================================================
// MSGEQ7
//================================================================================

CMSGEQ7 MSGEQ7;

CMSGEQ7::CMSGEQ7(void){
	// set pins to an invalid value to avoid writing to random pins if no begin() was called
	_resetPin = 0xFF;
	_strobePin = 0xFF;
	_analogPinLeft = 0xFF;
	_analogPinRight = 0xFF;
	_countRead = 0;
}

void CMSGEQ7::begin(uint8_t resetPin, uint8_t strobePin,
	uint8_t analogPinLeft, uint8_t analogPinRight){

	// initialize this instance's variables
	_resetPin = resetPin;
	_strobePin = strobePin;
	_analogPinLeft = analogPinLeft;

	// if right pin wasnt set just read the other twice
	if (analogPinRight == 0xFF)
		analogPinRight = analogPinLeft;
	_analogPinRight = analogPinRight;

	// do whatever is required to initialize the IC
	pinMode(_resetPin, OUTPUT);
	pinMode(_strobePin, OUTPUT);
	pinMode(_analogPinLeft, INPUT);
	pinMode(_analogPinRight, INPUT);
	reset();
}

void CMSGEQ7::reset(void){
	//only this setting seems to works properly
	digitalWrite(_strobePin, LOW);
	digitalWrite(_resetPin, HIGH);
	digitalWrite(_resetPin, LOW);
}

void CMSGEQ7::read(void){
	// increase counter. reset if it reaches bounds
	_countRead++;
	if (_countRead == MSGEQ7_READINGS) _countRead = 0;

	// reset the IC, optional, do this on your own if needed
	//reset(); 

	// read all 7 channels
	// 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
	for (int i = 0; i < 7; i++){
		// trigger next value
		digitalWrite(_strobePin, HIGH);
		digitalWrite(_strobePin, LOW);

		// allow the output to settle
		delayMicroseconds(36);

		// read pins with 8 bit resolution
		_left[i][_countRead] = analogRead(_analogPinLeft) >> 2;
		_right[i][_countRead] = analogRead(_analogPinRight) >> 2;
	}
}

uint8_t CMSGEQ7::get(uint8_t frequency, uint8_t channel){
	// dont read out of bounds
	if (frequency >= 7) return 0;

	// returns the average of the last MSGEQ7_READINGS
	uint16_t average = 0;

	for (int i = 0; i < MSGEQ7_READINGS; i++){
		// left channel
		if (channel == MSGEQ7_LEFT)
			average += _left[frequency][i];
		// right channel
		else if (channel == MSGEQ7_RIGHT)
			average += _right[frequency][i];
		// left + right channel
		else
			average += (_left[frequency][i] + _right[frequency][i]) / 2;
	}
	return average / MSGEQ7_READINGS;
}

uint8_t CMSGEQ7::peek(uint8_t frequency, uint8_t channel){
	// dont read out of bounds
	if (frequency >= 7) return 0;

	// returns the very last measurement

	// left channel
	if (channel == MSGEQ7_LEFT)
		return _left[frequency][_countRead];
	// right channel
	else if (channel == MSGEQ7_RIGHT)
		return _right[frequency][_countRead];
	// left + right channel
	else
		return uint16_t(_left[frequency][_countRead] + _right[frequency][_countRead]) / 2;
}
