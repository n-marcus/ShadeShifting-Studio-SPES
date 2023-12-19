ArrayList<Motor> motors = new ArrayList<Motor>();
int numMotors = 25;

class Motor {
  int index, x, y, w, h;
  float angle;

  //mode 0 = static, 1 = clockwise, 2 = counter clockwise;
  String[] modeNames = {"angle", "speed"};
  int mode = 0;

  int borderColorBase = 100;
  int borderColor = borderColorBase;

  int maxSpeed = 5;

  float speed = 1;

  boolean movingHome = false;

  boolean haveToSendOSCAfterScroll = false;

  int OSCIndicatorColor = 0;

  int heartBeatFill = 0;


  Motor(int _index, float _x, float _y) {
    index = _index;
    x = int(_x);
    y = int(_y);
    w = 100;
    h = 100;

    println("Created motor " + index + " at " + x + ", " + y);
  }

  void update() {
    updateRotation();
    drawMotor();
  }

  void scrollEnded() {
    if (haveToSendOSCAfterScroll) {
      //if we detected an end to a scrolling motion and this motor has changed
      //send the appropriate osc message
      sendOSCUpdate();
    }

    //reset the flag
    haveToSendOSCAfterScroll = false;
  }

  void updateRotation() {
    if (mode == 1) {
      angle += speed;
      angle %= 360;
    }
  }

  void drawMotor() {
    push();
    translate(x, y);
    fill(255);
    textSize(12);

    noFill();
    stroke(borderColor);
    strokeWeight(1);
    rect(0, 0, w, h);
    if (borderColor > borderColorBase) {
      borderColor -= 2;
    }

    textAlign(CENTER);
    text(index, w / 2, h * 0.1);

    text(modeNames[mode], w / 2, h * 0.2);


    if (mode == 0) {
      text("Angle:" + nf(angle, 0, 2), w / 2, h * 0.9);
    } else {
      text("Speed:" + nf(speed, 0, 2), w / 2, h * 0.9);
    }


    drawOSCIndicator();
    drawHeartBeat();

    translate(w / 2, h / 2);
    rotate(radians(-angle));
    rectMode(CENTER);
    stroke(255);
    noFill();
    rect(0, 0, 5, h * 0.5);
    fill(255);
    circle(0, h * 0.25, 10);


    pop();
  }

  void drawOSCIndicator() {

    if (OSCIndicatorColor > 0) {
      OSCIndicatorColor -= 4;
    }

    if (OSCIndicatorColor < 0) {
      OSCIndicatorColor = 0;
    }
    if (haveToSendOSCAfterScroll) {
      fill(0, 0, 255);
    } else {
      fill(OSCIndicatorColor);
    }
    circle(w - 10, 10, 10);
  }

  void drawHeartBeat() {
    //draw and fade the heartbeat
    if (heartBeatFill > 0) {
      //check if we need to fade
      heartBeatFill -= 2;
    } else {
      heartBeatFill = 0;
    }

    //draw heartbeat
    fill(heartBeatFill * 0.5, heartBeatFill * 0.5, heartBeatFill);
    circle(10, 10, 10);
  }

  void receivedHeartBeat() {
    heartBeatFill = 255;
  }
  
  void receivedHomeSignal() { 
    
  }  

  void handleScroll(float scrollSpeed) {
    scrolling = true;
    haveToSendOSCAfterScroll = true;

    if (mode == 0) {
      angle += scrollSpeed;
      angle = angle % 360;
    } else {
      speed += scrollSpeed * 0.1;
      speed = max( -maxSpeed, min(speed, maxSpeed));
    }
  }

  void resetPosition() {
    println("Motor " + index + " got reset");
    angle = 0;
    mode = 0;

    sendOSCUpdate();
  }

  void handleClick() {
    borderColor = 255;
    mode++;
    mode %= 2;

    sendOSCUpdate();
  }

  void resetToHome() {
    angle = 0;
    mode = 0;

    sendMotorHomeOSC();
  }

  void sendOSCUpdate() {
    if (mode == 1) {
      //if we switching to speed mode
      sendMotorSpeedOSC();
    } else if (mode == 0) {
      sendMotorAngleOSC();
    }

    //get the name of the mode
    String modeString = modeNames[mode];
    //get the value, default to angle
    float value = angle;
    if (mode == 1) {
      //if we are in speed mode, get the speed
      value = speed * 100.;
    }

    //save it to the hashmap
    editMotorData(currentScene, index, modeString, value);
  }

  void sendMotorHomeOSC() {
    String address = "/home";
    OscMessage myMessage = new OscMessage(address);

    myMessage.add(index); /* add an int to the osc message */
    oscP5.send(myMessage, myRemoteLocation);
    println("Sending message " + address + " " + index);


    OSCIndicatorColor = 255;
  }

  void sendMotorAngleOSC() {
    String address = "/moveToAngle";
    OscMessage myMessage = new OscMessage(address);

    myMessage.add(index); /* add an int to the osc message */
    myMessage.add(int(angle));
    oscP5.send(myMessage, myRemoteLocation);
    println("Sending message " + address + " " + index + " " + angle);


    OSCIndicatorColor = 255;
  }

  void sendMotorSpeedOSC() {
    String address = "/setSpeedAndDirection";
    OscMessage myMessage = new OscMessage(address);

    myMessage.add(index); /* add an int to the osc message */
    myMessage.add(int(speed * 1000));
    oscP5.send(myMessage, myRemoteLocation);
    println("Sending message " + address + " " + index + " " + speed * 100);
    OSCIndicatorColor = 255;
  }
}

void updateMotors() {
  for (int i = 0; i < numMotors; i ++) {
    Motor _motor = motors.get(i);
    _motor.update();
  }
}

void setupMotors(float gridXPos, float gridYPos, float gridWidth, float gridHeight) {
  //make a grid of motors
  for (int i = 0; i < numMotors; i++) {
    //calculate an x position
    float xPos = ((i % 5) * gridWidth / 5);
    xPos+= (gridWidth / 5) / 4;
    xPos+= gridXPos;

    //calculate an y position
    float yPos = floor(i / 5) * (gridHeight * 0.2);
    yPos+= gridYPos;

    //create themotor class
    motors.add(new Motor(i, xPos, yPos));
  }
}

void checkMotorsForScroll(float scrollSpeed) {
  for (int i = 0; i < numMotors; i ++) {
    Motor _motor = motors.get(i);
    if (mouseX > _motor.x && mouseX <  _motor.x +  _motor.w && mouseY >  _motor.y && mouseY <  _motor.y +  _motor.h) {
      _motor.handleScroll(scrollSpeed);
    }
  }
}

void checkMotorsForClick() {
  for (int i = 0; i < numMotors; i ++) {
    Motor _motor = motors.get(i);
    if (mouseX > _motor.x &&  mouseX <  _motor.x +  _motor.w && mouseY >  _motor.y && mouseY <  _motor.y +  _motor.h) {
      _motor.handleClick();
    }
  }
}

void resetMotorAtMouse() {
  for (int i = 0; i < numMotors; i ++) {
    Motor _motor = motors.get(i);
    if (mouseX > _motor.x &&  mouseX <  _motor.x +  _motor.w && mouseY >  _motor.y && mouseY <  _motor.y +  _motor.h) {
      _motor.resetPosition();
    }
  }
}

void homeMotorAtMouse() {
  for (int i = 0; i < numMotors; i ++) {
    Motor _motor = motors.get(i);
    if (mouseX > _motor.x &&  mouseX <  _motor.x +  _motor.w && mouseY >  _motor.y && mouseY <  _motor.y +  _motor.h) {
      _motor.resetToHome();
    }
  }
}

void resetAllMotorsToHome() {
  for (int i = 0; i < numMotors; i ++) {
    Motor _motor = motors.get(i);
    _motor.resetToHome();
    _motor.OSCIndicatorColor = 255;
  }
}

void sendMotorsScrollEndedEvent() {
  for (int i = 0; i < numMotors; i ++) {
    Motor _motor = motors.get(i);
    _motor.scrollEnded();
  }
}
