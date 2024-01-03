// How many leds in your strip?
#define NUM_LEDS 1

// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define DATA_PIN 16
// Define the array of leds
CRGB leds[NUM_LEDS];

void setupLED() {
  Serial.println("Setting up LED");
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  leds[0] = CRGB::Blue;
  FastLED.show();
  Serial.println("LED should be blue");
  delay(100);
}

void checkLED() {
  // fadeToBlackBy(leds, NUM_LEDS, 5);
  // FastLED.show();

  if (cycleCount % 711 == 0) {
    if (isPlaying) {
      if (sceneMode == 0) {
        leds[0] = CRGB::Yellow;
      } else if (sceneMode == 1) {
        leds[0] = CRGB::Green;
      }
    } else {
      if (millis() % 1000 < 500) {
        leds[0] = CRGB::Red;
      } else {
        leds[0] = CRGB::Black;
      }
    }
    FastLED.show();
  }
}