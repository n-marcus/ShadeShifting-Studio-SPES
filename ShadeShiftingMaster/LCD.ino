#include <Arduino.h>
#include <Wire.h>
#include "LiquidCrystal_PCF8574.h"


TwoWire Wire_1 = TwoWire();


LiquidCrystal_PCF8574 lcd(0x27);  // set the LCD address to 0x27
char lcdDisplay[2][16];           // 4 lines of 20 character buffer

void setupLCD() {
  Serial.println("Starting LCD display setup");
  Wire_1.begin(D1, D2);                 // custom i2c port on ESP
  Wire_1.setClock(100000);              // standard 100kHz speed
  Wire_1.setClockStretchLimit(200000);  // some devices might need clock stretching
  lcd.begin(20, 4, Wire_1);
  lcd.setBacklight(255);
  Serial.println("Displaying Morgen schat...");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("morgen schat! <3");
  delay(1500);
  lcd.clear();
}

void checkLCD() {
  cycleCount++;

  if (cycleCount % 5000 == 0) {

    lcd.clear();
    if (sceneMode == 0) {
      //if we are  busy waiting for the motors to reset
      lcd.setCursor(0, 0);
      lcd.print("Resetting");

      //count how many motors told us they have resetted already
      int numMotorsResetted = 0;
      for (int i = 0; i < NUM_MOTORS; i++) {
        if (motorsResetted[i]) {
          numMotorsResetted++;
        }
      }

      // Serial.println("Num motors resetted is " + String(numMotorsResetted));
      lcd.setCursor(0, 1);
      if (isPlaying) {
        //only display remaining time if we are actually playing, otherwise it doesnt matter anyway
        int resetTime = (RESET_TIMEOUT - timeResetting) / 1000;
        lcd.print(resetTime);
        lcd.print("s");
      }
      lcd.print(" done:");
      lcd.print(numMotorsResetted);
    } else if (sceneMode == 1) {
      lcd.setCursor(0, 0);
      lcd.print("Scene = " + String(currentScene));
      lcd.setCursor(0, 1);
      // String lcdString = String(int((TIME_PER_SCENE_MS - timeInCurrentScene) / 1000) + " s");
      // Serial.println((TIME_PER_SCENE_MS - timeInCurrentScene) / 1000);

      int timeLeftInScene = (TIME_PER_SCENE_MS - timeInCurrentScene) / 1000;
      if (isPlaying) {
        lcd.print(timeLeftInScene);
        lcd.print("s");
      }
    }

    String playingStatus = (isPlaying) ? ">" : "|";
    lcd.setCursor(15, 0);
    lcd.print(playingStatus);

    checkActiveMotors();
    lcd.setCursor(12, 1);
    //this leaves 4 characters on the bottom right
    lcd.print("ON");
    lcd.print(numActiveMotors);
  }
}

void setLCDResetting() {
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Resetting nodes...");
}


void setLCDCurrentScene() {
  // lcd.clear();
  // lcd.setCursor(0, 0);
  // lcd.print("Scene: " + String(currentScene));
}
