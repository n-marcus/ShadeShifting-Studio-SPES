

void checkOSC() {
  OSCMessage msg;
  int size = Udp.parsePacket();

  if (size > 0) {
    while (size--) {
      msg.fill(Udp.read());
    }
    if (!msg.hasError()) {
      sinceOSC = 0;
      checkLED();
      msg.dispatch("/moveToAngle", moveToAngleOSC);
      msg.dispatch("/setSpeedAndDirection", setSpeedAndDirectionOSC);
      msg.dispatch("/home", moveToHomeOSC);


    } else {
      error = msg.getError();
      Serial.print("error: ");
      Serial.println(error);
    }
  }
}

void setSpeedAndDirectionOSC(OSCMessage &msg) {
  int node = msg.getInt(0);
  int speed = msg.getInt(1);
  Serial.println("Got set speed command for node " + String(node) + " from OSC at " + String(speed));
  //if it is for this node, or for all nodes
  if (node == NODE_NUMBER || node == -1) {
    targetSpeed = speed;
    // currentSpeed = 0;
    // stepper.setSpeed(speed);  // Set the speed of the motor (adjust as needed)
    motorRunning = true;
    motorRunningSpeed = true;
  } else {
    Serial.println("Got move to angle osc message but it was not meant for me, was meant for: " + String(node));
  }
}

void moveToAngleOSC(OSCMessage &msg) {
  //check which node this message was supposed to go to
  int node = msg.getInt(0);
  int angle = msg.getInt(1);
  Serial.println("Got set angle command for node " + String(node) + " from OSC with angle " + String(angle));

  //if it is for this node, or for all nodes
  if (node == NODE_NUMBER || node == -1) {
    //this is a bit arbitrary, but might be usefull in the future to minimize travel distances.
    float currentCirclesMade = float(stepper.currentPosition()) / float(stepsNeededForCircle);
    float currentAngleModulo = stepper.currentPosition() % stepsNeededForCircle;
    Serial.println("Current circles made is " + String(currentCirclesMade) + " modulo is " + currentAngleModulo);
    stepper.setAcceleration(DEFAULT_ACCELERATION);

    //move to the specified angle
    stepper.moveTo(stepsPerDegree * angle);

    //flags to update appropriately
    motorRunning = true;
    motorRunningSpeed = false;
  } else {
    Serial.println("Got move to angle osc message but it was not meant for me, was meant for: " + String(node));
  }
}

void moveToHomeOSC(OSCMessage &msg) {
  Serial.println("Got OSC message to move to home");
  homing();
}