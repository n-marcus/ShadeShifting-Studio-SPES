void setupSerial() {
  Serial.begin(9600);
  delay(100);
  DEBUG_PRINT("");
  DEBUG_PRINT("- - - - - - - - - ");
  DEBUG_PRINT("This is node number " + String(NODE_NUMBER));
  DEBUG_PRINT("- - - - - - - - - ");
}

void checkSerial() {
  if (Serial.available() > 0) {         // Check if data is available to read
    char receivedChar = Serial.read();  // Read the incoming byte

    if (receivedChar == 'e') {         // Check if the received character is 'e'
      DEBUG_PRINT("Received 'e'");  // Send a confirmation message
      stepper.setAcceleration(500);
      DEBUG_PRINT("Moving to " + String(stepper.currentPosition() + 500));
      stepper.moveTo(stepper.currentPosition() + 1000);
      motorRunning = true;
    }

    if (receivedChar == 'r') {         // Check if the received character is 'e'
      DEBUG_PRINT("Received 'e'");  // Send a confirmation message
      stepper.setAcceleration(500);
      DEBUG_PRINT("Moving to " + String(stepper.currentPosition() + stepsNeededForCircle));
      stepper.moveTo(stepper.currentPosition() + stepsNeededForCircle);
      motorRunning = true;
    }


    if (receivedChar == 'w') {         // Check if the received character is 'e'
      DEBUG_PRINT("Received 'w'");  // Send a confirmation message
      stepper.setAcceleration(500);
      stepper.moveTo(stepper.currentPosition() - 1000);
      motorRunning = true;
    }

    if (receivedChar == 'h') {         // Check if the received character is 'e'
      DEBUG_PRINT("Received 'h'");  // Send a confirmation message
      //Return to home
      returnMotorToHome();
      motorRunning = true;
    }

    if (receivedChar == 'd') {  // Check if the received character is 'd'
      int number = 0;
      while (Serial.available() > 0) {
        char digit = Serial.read();
        if (digit >= '0' && digit <= '9') {
          number = number * 10 + (digit - '0');
        } else {
          break;  // Break if non-numeric character is encountered
        }
      }

      // Perform actions using the extracted number
      // For example, you can call a function and pass 'number' as an argument
      // Here, let's just print the extracted number
      DEBUG_PRINT("Received number: ");
      DEBUG_PRINT(number);
    }
  }

}
