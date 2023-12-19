OscP5 oscP5;
NetAddress myRemoteLocation;

void setupOSC() {
  oscP5 = new OscP5(this, 8001);
  myRemoteLocation = new NetAddress("255.255.255.255", 8888);
  oscP5.plug(this, "gotHeartBeat", "/heartbeat");
  oscP5.plug(this, "gotHomeSignal", "/reachedHome");
}

void mouseDragged() {

  OscMessage myMessage = new OscMessage("/test");

  myMessage.add(mouseX); /* add an int to the osc message */
  oscP5.send(myMessage, myRemoteLocation);

  println("I have send a message " + mouseX );
}

void sendResetToHomeOSC() {
  OscMessage myMessage = new OscMessage("/home");

  oscP5.send(myMessage, myRemoteLocation);
}

void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  //print("### received an osc message.");
  //print(" addrpattern: "+theOscMessage.addrPattern());

  //if (theOscMessage.addrPattern() == "heartBeat") {
  //  println(" typetag: "+theOscMessage.typetag());
  //  if (theOscMessage.typetag() == 'i') {
  //    int node = theOscMessage.get()
  //  }
  //}
}


void gotHomeSignal(int node) {
  println("Got home signal from " + node);
  try {
    Motor _motor = motors.get(node);
    _motor.receivedHomeSignal();
  }
  catch (Exception e) {
    print("Could not send home signal to motor class " + node);
  }
}

void gotHeartBeat(int node) {
  println("Got heartbeat from "+ node);
  try {
    Motor _motor = motors.get(node);
    _motor.receivedHeartBeat();
  }
  catch (Exception e) {
    print("Could not send heartbeat to motor class " + node);
  }
}
