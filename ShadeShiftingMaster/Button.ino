#define BUTTON_PIN 12

// Button button(BUTTON_PIN); // Connect your button between pin 4 and GND

Adafruit_Debounce button(BUTTON_PIN, LOW);

void setupButton() {
  // pinMode(BUTTON_PIN, INPUT_PULLUP);
  button.begin();
}

void checkButton() {
  // if (button.pressed()) {
  //   Serial.println("BUTTON PRESSED");
  // }

  if (cycleCount % 100 == 0) {

    button.update();

    if (button.justPressed()) {
      Serial.println("Button was just pressed!");
      if (isPlaying) {
        pause();
      } else {
        start();
      }
    }
  }

  // if (button.justReleased()) {
  //   Serial.println("Button was just released!");
  // }
  // Serial.println(digitalRead(BUTTON_PIN));
}