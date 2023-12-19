void runMotor() {
  if (motorRunning) {
    //check if we are running in constant or variable speed
    if (motorRunningSpeed) {

      //check if we have to change the speed
      if (currentSpeed < targetSpeed) {
        currentSpeed += speedAcceleration;
        //increase speed if necesary
        stepper.setSpeed(currentSpeed);
      } else if (currentSpeed > targetSpeed) {
        //decrease speed if necesary
        currentSpeed -= speedAcceleration;
        stepper.setSpeed(currentSpeed);
      }
      stepper.runSpeed();
    } else {
      //run with constant speed
      stepper.run();
    }
  }
}

void returnMotorToHome() {
  DEBUG_PRINT("Starting home finding procedure...");


  //show green led when found home
  // leds[0] = CRGB::Yellow;
  // FastLED.show();
  // delay(10);
  Serial.println("LED should be yellow now");

  stepper.setAcceleration(5000);
  stepper.moveTo(stepper.currentPosition() + stepsNeededForCircle);
  // stepper.setSpeed(500);
  int loopIndex = 0;
  bool reachedHome = false;
  while (digitalRead(HALSensorPin) == 1) {
    stepper.run();
    delay(1);
  }
  stepper.stop();
  motorRunning = false;

  //show green led when found home
  leds[0] = CRGB::Green;
  FastLED.show();
  delay(10);

  DEBUG_PRINT("Found home!");
  DEBUG_PRINT("Position before reset = " + String(stepper.currentPosition()));
  stepper.setCurrentPosition(0);
  DEBUG_PRINT("Position after reset = " + String(stepper.currentPosition()));

  sendHomeSignalOSC();
}
