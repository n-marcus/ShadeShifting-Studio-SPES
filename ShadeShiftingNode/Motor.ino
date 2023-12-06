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

void homing() {
  Serial.println("Starting home finding procedure...");

  stepper.setAcceleration(5000);
  stepper.moveTo(stepper.currentPosition() + stepsNeededForCircle);
  // stepper.setSpeed(500);
  int loopIndex = 0;
  bool reachedHome = false;
  while (digitalRead(HALSensorPin) == 1) {
    stepper.run();
    delay(1);
    // if (loopIndex % 100 == 0) {
    //   //check every 100 loops if we reached the sensor
    //   reachedHome = !digitalRead(HALSensorPin);
    // }
    // loopIndex++;
  }
  stepper.stop();
  motorRunning = false;

  Serial.println("Found home!");
  Serial.println("Position before reset = " + String(stepper.currentPosition()));
  stepper.setCurrentPosition(0);
  Serial.println("Position after reset = " + String(stepper.currentPosition()));
}
