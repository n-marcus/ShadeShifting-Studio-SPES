import netP5.*;
import oscP5.*;
import controlP5.*;

OscP5 oscP5;
NetAddress myRemoteLocation;
NetAddress masterLocation;
int sceneMode, currentScene, isPlaying, timeLeft, numMotorsResetted;

int textSize = 24;

void setup() {

  oscP5 = new OscP5(this, 8888);
  //myRemoteLocation = new NetAddress("255.255.255.255", 8888);
  //masterLocation = new NetAddress("255.255.255.255", 8001);

  oscP5.plug(this, "masterNodeUpdate", "/masterNodeUpdate");
  size(400, 400);
  pixelDensity(2);
  background(00);
}


void draw() {

  background(0);
  textSize(textSize);

  fill(255);
  pushMatrix();
  
  translate(0, 100);
  text("Current scene: " + str(currentScene), 0, textSize);
  String modeString = (sceneMode == 0) ? "resetting..." : "active";
  text("Current mode: " +modeString, 0, textSize * 2);
  String playingString = isPlaying == 0 ? "Paused" : "Playing";
  text("Playing: " + playingString, 0, textSize * 3);
  text("Time left: " + timeLeft, 0, textSize * 4);
  if (sceneMode == 0) {
    text("Motors resetted: " + numMotorsResetted, 0, textSize * 5);
  }
  
  popMatrix();
}

void masterNodeUpdate(int _currentScene, int _sceneMode, int _isPlaying, int _timeLeft, int _numMotorsResetted) {
  println("Got master node update "  + _currentScene + " in mode " + _sceneMode + " playing is " + _isPlaying);

  currentScene = _currentScene;
  sceneMode = _sceneMode;
  isPlaying = _isPlaying;
  timeLeft = _timeLeft;
  numMotorsResetted = _numMotorsResetted;
}

void oscEvent(OscMessage theOscMessage) {
  /* print the address pattern and the typetag of the received OscMessage */
  //print("### received an osc message.");
  //print(" addrpattern: "+theOscMessage.addrPattern());
}
