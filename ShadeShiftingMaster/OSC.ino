
elapsedMillis sinceOSC;
char receivedScene[300];

bool motorsResetted[NUM_MOTORS];

void sendRandomRotationToNode(int node) {
  OSCMessage msg("/moveToAngle");
  msg.add(node);
  msg.add(int(random(360)));
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}

void checkOSC() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    // DEBUG_PRINT("There is something in the osc buffer...");
    while (size--) {
      msg.fill(Udp.read());
    }

    if (!msg.hasError()) {
      sinceOSC = 0;
      // DEBUG_PRINT("Received OSC message succesfully:");

      // msg.dispatch("/moveToAngle", moveToAngleOSC);
      msg.dispatch("/heartbeat", gotHeartbeat);
      msg.dispatch("/reachedHome", gotReachedHome);
      msg.dispatch("/scene", gotSceneOSC);


    } else {
      DEBUG_PRINT("Error receiving OSC message: ");
      DEBUG_PRINT(msg.getError());
    }
  }
}


void gotSceneOSC(OSCMessage &msg) {
  int scene = msg.getInt(0);
  msg.getString(1, receivedScene);
  DEBUG_PRINT("Received scene " + String(scene) + " over OSC, starting decoding...");
  DEBUG_PRINT(receivedScene);
  decodeSceneString(receivedScene, scene);
  DEBUG_PRINT("Done decoding");
}

void gotHeartbeat(OSCMessage &msg) {
  int node = msg.getInt(0);
  // DEBUG_PRINT("Got heartbeat from node " + String(node));
}

void gotReachedHome(OSCMessage &msg) {
  int node = msg.getInt(0);
  DEBUG_PRINT("Got reached home from node " + String(node));

  //save that this motor has been resetted
  if (node > 0 && node < NUM_MOTORS) {
    motorsResetted[node] = true;
  }
}

void sendDataForMotorInScene(int scene, int motor) {
  int mode = scenes[scene].motors[motor].mode;
  float value = scenes[scene].motors[motor].value;

  if (mode == 1) { 
    value = value * 1000.;
  }
  // DEBUG_PRINT("Sending OSC to " + String(motor) + " mode: " + String(mode) + " value: " + String(value));
  String OSCaddress = "";

  if (mode == 0) {
    OSCMessage msg("/moveToAngle");
    msg.add(motor);
    msg.add(int(value));
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    DEBUG_PRINT("Send /moveToAngle " + String(motor) + " " + String(value));
  } else if (mode == 1) {
    OSCMessage msg("/setSpeedAndDirection");
    msg.add(motor);
    msg.add(int(value));
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    DEBUG_PRINT("Send /setSpeedAndDirection " + String(motor) + " " + String(value));
  }
}

void decodeSceneString(String input, int sceneNumber) {

  // Temporary variables to store motor information
  int motorNumber = 0;
  int mode = 0;
  float value = 0.0;

  int startIndex = 0;
  int endIndex = 0;

  // Loop through the input string
  while (endIndex != -1) {
    // Find the position of the next 'm' character
    endIndex = input.indexOf('m', startIndex + 1);

    // If 'm' is found, extract the substring between startIndex and endIndex
    String motorData;
    if (endIndex != -1) {
      motorData = input.substring(startIndex, endIndex);
    } else {
      // If 'm' is not found, extract the substring from startIndex to the end of the string
      motorData = input.substring(startIndex);
    }

    // Parse motorData to extract motorNumber, mode, and value
    sscanf(motorData.c_str(), "m%d-%d-%f", &motorNumber, &mode, &value);

    // Print the extracted values for each motor (for demonstration)
    Serial.print("Motor: ");
    Serial.print(motorNumber);
    Serial.print(", Mode: ");
    Serial.print(mode);
    if (mode == 1) {
      value = value / 100.;
    }
    Serial.print(", Value: ");
    Serial.println(value);

    if (sceneNumber >= 0 && sceneNumber < NUM_SCENES) {
      if (motorNumber >= 0 && motorNumber < NUM_MOTORS) {
        //save the received value to memory
        scenes[sceneNumber].motors[motorNumber].mode = mode;
        scenes[sceneNumber].motors[motorNumber].value = value;
      }
    } else { 
      Serial.println("You tried writing to scene " + String(sceneNumber) + " and motor " + String(motorNumber) + " and I cannot allow that my brother in Christ");
    }

    // Update the startIndex for the next motor data
    startIndex = endIndex;
  }

  saveScenesToEEPROM();

  loadScenesFromEEPROM();

  printMotorData(false, sceneNumber);
}
