void checkOSC() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    DEBUG_PRINT("There is something in the osc buffer...");
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      sinceOSC = 0;
      DEBUG_PRINT("Received OSC message succesfully:");
      checkLED();
      msg.dispatch("/moveToAngle", moveToAngleOSC);
      msg.dispatch("/setSpeedAndDirection", setSpeedAndDirectionOSC);
      msg.dispatch("/home", moveToHomeOSC);


    } else {
      error = msg.getError();
      Serial.print("Error receiving OSC message: ");
      DEBUG_PRINT(error);
    }
  }
}

void setSpeedAndDirectionOSC(OSCMessage &msg) {
  int node = msg.getInt(0);
  int speed = msg.getInt(1);
  DEBUG_PRINT("Got set speed command for node " + String(node) + " from OSC with speed " + String(speed));
  //if it is for this node, or for all nodes
  if (node == NODE_NUMBER || node == -1) {
    targetSpeed = speed;
    // currentSpeed = 0;
    // stepper.setSpeed(speed);  // Set the speed of the motor (adjust as needed)
    motorRunning = true;
    motorRunningSpeed = true;
  } else {
    DEBUG_PRINT("Got move to angle osc message but it was not meant for me, was meant for: " + String(node));
  }
}

void moveToAngleOSC(OSCMessage &msg) {
  int node = msg.getInt(0);
  int angle = msg.getInt(1);

  if (node == NODE_NUMBER || node == -1) {
    DEBUG_PRINT("Got set angle command for node " + String(node) + " from OSC with angle " + String(angle));
    // Get the current position of the motor
    int currentPosition = stepper.currentPosition();

    // Calculate the current angle
    float currentCirclesMade = float(currentPosition) / float(stepsNeededForCircle);
    float currentAngle = currentPosition % stepsNeededForCircle;
    DEBUG_PRINT("Current circles made is " + String(currentCirclesMade) + " modulo is " + currentAngle);

    // Calculate the shortest distance to the target angle
    int targetPosition = stepsPerDegree * angle;
    int distanceClockwise = (targetPosition - currentPosition + stepsNeededForCircle) % stepsNeededForCircle;
    int distanceCounterClockwise = (currentPosition - targetPosition + stepsNeededForCircle) % stepsNeededForCircle;

    // Determine the shortest distance and move the motor accordingly
    int stepsToMove = (distanceClockwise <= distanceCounterClockwise) ? distanceClockwise : -distanceCounterClockwise;
    int targetSteps = currentPosition + stepsToMove;

    // Move to the specified angle
    stepper.moveTo(targetSteps);

    // Flags to update appropriately
    stepper.setAcceleration(DEFAULT_ACCELERATION);
    motorRunning = true;
    motorRunningSpeed = false;
    currentSpeed = 0;
  } else {
    DEBUG_PRINT("Got move to angle osc message but it was not meant for me, was meant for: " + String(node));
  }
}

void moveToHomeOSC(OSCMessage &msg) {
  int node = msg.getInt(0);
  DEBUG_PRINT("Got set home command for node " + String(node) + " from OSC");

  if (node == NODE_NUMBER || node == -1) {
    DEBUG_PRINT("Starting home");
    homing();
  } else {
    DEBUG_PRINT("Got set home command but not for me, for node " + String(node));
  }
}