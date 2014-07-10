/*
 Copyright (c) 2014 NicoHood
 See the readme for credit to other people.

 MSGEQ7 Serial example
 Output via led strip

 Reads MSGEQ7 IC with 7 different frequencys from range 0-255
 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/

// FastLed v2.1
#include <FastLED.h>
const int numLeds = 50;
const int pinData = 11;
const int  pinClock = 13;
CRGB leds[numLeds];

//MSGEQ7
#include <MSGEQ7.h>
const int pinAnalogLeft = A0;
const int pinAnalogRight = A1;
const int pinReset = A2;
const int pinStrobe = A3;

void setup() {
  // Uncomment/edit one of the following lines for your leds arrangement.
  // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<NEOPIXEL, DATA_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);

  FastLED.addLeds<WS2801, RGB>(leds, numLeds);
  // FastLED.addLeds<SM16716, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD8806, GRB>(leds, NUM_LEDS);

  // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
  // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, GRB>(leds, NUM_LEDS);

  FastLED.setDither(0); // prevent flickering
  FastLED.show(); // needed!

  // this will set the IC ready for reading
  MSGEQ7.begin(pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight);

  // debug
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

    // this should show different sytax to get the values from the IC

    // LOW and BASS is the name = 0
    // use LEFT and RIGHT for the different channels
    uint8_t leftbass = MSGEQ7.get(MSGEQ7_BASS, MSGEQ7_LEFT);
    uint8_t rightbass = MSGEQ7.get(MSGEQ7_LOW, MSGEQ7_RIGHT);

    // MID is equal to 3
    // you can use peek for a not smoothed value
    uint8_t leftpeekmid = MSGEQ7.peek(MSGEQ7_MID, MSGEQ7_LEFT);
    uint8_t rightpeekmid = MSGEQ7.peek(3, MSGEQ7_RIGHT);

    // you can use LEFT_RIGHT or leave it out to get both channels together
    // use 0-6 for all frequencys
    uint8_t high  = MSGEQ7.get(MSGEQ7_HIGH, MSGEQ7_LEFT_RIGHT);
    uint8_t veryhigh = MSGEQ7.get(6);

    // smoothed values look way better, channel 3 and 4 are similar
    uint8_t leftmid = MSGEQ7.get(4, MSGEQ7_LEFT);
    uint8_t rightmid = MSGEQ7.get(4, MSGEQ7_RIGHT);

    // visualize leds
    // for better visualizations see "Visualizer" on Github
    for (int i = 0; i < 5; i++) {
      leds[i].setHSV(HUE_RED, 255, leftbass);
      leds[i + 5].setHSV(HUE_ORANGE, 255, rightbass);
      leds[i + 10].setHSV(HUE_YELLOW, 255, leftpeekmid);
      leds[i + 15].setHSV(HUE_GREEN, 255, rightpeekmid);
      leds[i + 20].setHSV(HUE_AQUA, 255, high);
      leds[i + 25].setHSV(HUE_BLUE, 255, veryhigh);
      leds[i + 30].setHSV(HUE_PURPLE, 255, leftmid);
      leds[i + 35].setHSV(HUE_PINK, 255, rightmid);
    }

    // You can also calculate an overall volume level
    // but thats more or less useless
    uint16_t average = 0;
    for (int i = 0; i < 7; i++)
      average += MSGEQ7.get(i);
    uint8_t overall = average / 7;
    CRGB color = 0xFFAA11;
    color.nscale8_video(overall);
    for (int i = 0; i < 10; i++)
      leds[i + 40] = color;

    // update leds
    FastLED.show();
  }
}
