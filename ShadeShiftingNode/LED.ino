#define DATA_PIN 2
#define LED_TYPE WS2811
#define COLOR_ORDER RGB
#define NUM_LEDS 1
CRGB leds[NUM_LEDS];
#define BRIGHTNESS 2

int cycleCount = 0;

int hue = 0;

void checkLED() {
  // if (sinceOSC > 100) {
  //   digitalWrite(LED_BUILTIN, HIGH);
  // } else {
  //   digitalWrite(LED_BUILTIN, LOW);
  // }
}

void setupLEDs() {
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

void showLEDs() {
  // send the 'leds' array out to the actual LED strip
  // leds[0] = CHSV(hue,200,255);
  if (cycleCount % 500 == 0) {
    // Serial.println("show led");
    fadeToBlackBy(leds, NUM_LEDS, 10);
    FastLED.show();
  }
  cycleCount ++;
  // hue += 1;
  // hue = hue % 360;
}