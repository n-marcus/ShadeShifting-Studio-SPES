void checkSerial() {
  if (Serial.available()) {             // Check if data is available to read
    char receivedChar = Serial.read();  // Read the incoming byte
    if (isdigit(receivedChar)) {
      int receivedNumber = receivedChar - '0';  // Convert char to integer

      // Print the received number to the Serial Monitor
      Serial.print("Received Number: ");
      Serial.println(receivedNumber);

      currentScene = receivedNumber;
      printMotorData(false, receivedNumber);
      setSceneActive();
    } else if (receivedChar == 'p' || receivedChar == 'P') {
      // If 'p' or 'P' is received
      Serial.println("Received 'p'");
      pause();
      // Handle 'p' case here
    } else if (receivedChar == 's' || receivedChar == 'S') {
      // If 's' or 'S' is received
      Serial.println("Received 's'");
      start();
      // Handle 's' case here
    }
  }
}