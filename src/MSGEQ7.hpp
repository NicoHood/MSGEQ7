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


//================================================================================
// Tools
//================================================================================

MSGEQ7_data_t mapNoise(MSGEQ7_data_t x, MSGEQ7_data_t in_min, MSGEQ7_data_t in_max, MSGEQ7_data_t out_min, MSGEQ7_data_t out_max)
{
	// if input is smaller/bigger than expected return the min/max out ranges value
	if (x < in_min)
		return out_min;
	else if (x > in_max)
		return out_max;

	// map the input to the output range.
	// round up if mapping bigger ranges to smaller ranges
	else  if ((in_max - in_min) > (out_max - out_min))
		return (uint16_t)(x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1) + out_min;
	// round down if mapping smaller ranges to bigger ranges
	else
		return (uint16_t)(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//================================================================================
// MSGEQ7
//================================================================================

// initialize the instance's variables
template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
CMSGEQ7<smooth, resetPin, strobePin, firstAnalogPin, analogPins ...>::
CMSGEQ7(void)
{
	// empty
}


//================================================================================
// IC setup
//================================================================================

template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
void CMSGEQ7<smooth, resetPin, strobePin, firstAnalogPin, analogPins ...>::
begin(void){
	// do whatever is required to initialize the IC
	pinMode(resetPin, OUTPUT);
	pinMode(strobePin, OUTPUT);
	pinMode(firstAnalogPin, INPUT);
	nop((pinMode(analogPins, INPUT), 0)...);
}

template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
void CMSGEQ7<smooth, resetPin, strobePin, firstAnalogPin, analogPins ...>::
end(void){
	// set pins to input again to safely remove connections if needed
	pinMode(resetPin, INPUT);
	pinMode(strobePin, INPUT);
	pinMode(firstAnalogPin, INPUT);
	nop((pinMode(analogPins, INPUT), 0)...);
}

template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
void CMSGEQ7<smooth, resetPin, strobePin, firstAnalogPin, analogPins ...>::
reset(void){
	// only this setting seems to works properly
	digitalWrite(strobePin, LOW);
	digitalWrite(resetPin, HIGH);
	digitalWrite(resetPin, LOW);
}


//================================================================================
// read
//================================================================================

template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
bool CMSGEQ7<smooth, resetPin, strobePin, firstAnalogPin, analogPins ...>::
read(void){
	// reset the IC frequently, otherwise it will get out of sync after a while
	reset();

	// read all 7 channels
	// 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
	for (uint8_t i = 0; i < 7; i++){
		// trigger next value
		digitalWrite(strobePin, HIGH);
		digitalWrite(strobePin, LOW);

		// allow the output to settle
		delayMicroseconds(36);

		// read pins with 8 bit resolution
		frequency f = (frequency){
#ifdef MSGEQ7_10BIT
			analogRead(firstAnalogPin),
				analogRead(analogPins)...
#else
			MSGEQ7_data_t(analogRead(firstAnalogPin) >> 2),
				MSGEQ7_data_t(analogRead(analogPins) >> 2)...
#endif
		};

		// Save smooth values
		if (smooth){
			for (uint8_t p = 0; p < (1 + sizeof...(analogPins)); p++){
				// Round up if value increases
				if (frequencies[i].pin[p] < f.pin[p])
					frequencies[i].pin[p]++;

				// Smooth value
				frequencies[i].pin[p] = uint16_t(frequencies[i].pin[p] * smooth + f.pin[p] * (255 - smooth)) / 255;
			}
		}
		// Save peek values
		else
			frequencies[i] = f;

	}
	return true;
}

template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
bool CMSGEQ7<smooth, resetPin, strobePin, firstAnalogPin, analogPins ...>::
read(const uint32_t interval){
	// Read without delay
	// TODO use static variable??
	static uint32_t prevMicros = 0;
	uint32_t currentMicros = micros();
	if ((currentMicros - prevMicros) > interval) {
		prevMicros = currentMicros;

		// Analyze
		read();
		return true;
	}
	return false;
}


//================================================================================
// get values
//================================================================================

template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
MSGEQ7_data_t CMSGEQ7<smooth, resetPin, strobePin, firstAnalogPin, analogPins ...>::
get(const uint8_t frequency, const uint8_t channel){
	// dont read out of bounds
	if (frequency >= 7 || channel >= (1 + sizeof...(analogPins)))
		return 0;

	// returns the last measurement
	return frequencies[frequency].pin[channel];
}

template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
MSGEQ7_data_t CMSGEQ7<smooth, resetPin, strobePin, firstAnalogPin, analogPins ...>::
get(const uint8_t frequency){
	// dont read out of bounds
	if (frequency >= 7)
		return 0;

	// go through all channels/pins and add measured values
	uint16_t average = 0;
	for (uint8_t p = 0; p < (1 + sizeof...(analogPins)); p++)
		average += frequencies[frequency].pin[p];

	// return the average of all channels
	return (average / (1 + sizeof...(analogPins)));
}

template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
MSGEQ7_data_t CMSGEQ7<smooth, resetPin, strobePin, firstAnalogPin, analogPins ...>::
getVolume(uint8_t channel){
	// dont read out of bounds
	if (channel >= (1 + sizeof...(analogPins)))
		return 0;

	// add all frequencies of a single channel to the overall volume
	uint16_t vol = 0;
	for (uint8_t i = 0; i < 7; i++)
		vol += frequencies[i].pin[channel];

	// return the average of this channels
	return vol / 7;
}

template <uint8_t smooth, uint8_t resetPin, uint8_t strobePin, uint8_t firstAnalogPin, uint8_t ...analogPins>
MSGEQ7_data_t CMSGEQ7<smooth, resetPin, strobePin, firstAnalogPin, analogPins ...>::
getVolume(void){
	// add all frequencies of all channels to the overall volume
	uint16_t vol = 0;
	for (uint8_t i = 0; i < 7; i++)
		vol += get(i);

	// return the average of all channels
	return vol / 7;
}
