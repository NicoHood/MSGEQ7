/*
Copyright (c) 2014-2015 NicoHood
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

// include guard
#pragma once

// software version
#define MSGEQ7_VERSION 120

// needed for digitalRead(), analogRead() and micros()
#include <Arduino.h>

//================================================================================
// MSGEQ7
//================================================================================

//#define MSGEQ7_10BIT
#define ReadsPerSecond(f) (1000000UL / (f))

// basic frequencys definitions (0-6 valid, 7 channels)
#define MSGEQ7_0 0
#define MSGEQ7_1 1
#define MSGEQ7_2 2
#define MSGEQ7_3 3
#define MSGEQ7_4 4
#define MSGEQ7_5 5
#define MSGEQ7_6 6
#define MSGEQ7_BASS 0
#define MSGEQ7_LOW 0
#define MSGEQ7_MID 3
#define MSGEQ7_HIGH 5

// resolution dependant settings
#ifdef MSGEQ7_10BIT
typedef uint16_t MSGEQ7_data_t;
#define MSGEQ7_IN_MIN 20
#define MSGEQ7_IN_MAX 1023
#define MSGEQ7_OUT_MIN 0
#define MSGEQ7_OUT_MAX 1023
#else
typedef uint8_t MSGEQ7_data_t;
#define MSGEQ7_IN_MIN 20
#define MSGEQ7_IN_MAX 255
#define MSGEQ7_OUT_MIN 0
#define MSGEQ7_OUT_MAX 255
#endif

template <bool smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
class CMSGEQ7{
public:
	CMSGEQ7(void);

	// functions to get the IC ready for reading
	void begin(void);
	void end(void);
	void reset(void);

	// functions to read the IC values and save them to the internal array
	bool read(void);
	bool read(const uint32_t currentMicros, const uint32_t interval);
	bool read(const uint32_t interval);

	// function for the user to access the values
	MSGEQ7_data_t get(const uint8_t frequency, const uint8_t channel);
	MSGEQ7_data_t get(const uint8_t frequency);
	MSGEQ7_data_t getVolume(uint8_t channel);
	MSGEQ7_data_t getVolume(void);

	// tools
	MSGEQ7_data_t map(MSGEQ7_data_t x, MSGEQ7_data_t in_min = MSGEQ7_IN_MIN, MSGEQ7_data_t in_max = MSGEQ7_IN_MAX,
		MSGEQ7_data_t out_min = MSGEQ7_OUT_MIN, MSGEQ7_data_t out_max = MSGEQ7_OUT_MAX);

private:
	// array of all input values
	typedef struct frequency{
		MSGEQ7_data_t pin[1 + sizeof...(analogPins)];
	};
	frequency frequencies[7];

	void nop(...) {
		// little hack to take as many arguments as possible
		// to execute several functions for the analogPins
	}
};

// implementation inline, moved to another .hpp file
#include "MSGEQ7.hpp"
