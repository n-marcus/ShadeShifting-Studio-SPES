import netP5.*;
import oscP5.*;
import controlP5.*;




void setup() {
  size(800, 800);
  pixelDensity(2);
  smooth();
  setupOSC();
  setupControlBar();
  setupMotors(0, controlBarHeight, width, height - controlBarHeight );
}

void draw() {
  background(0, 0, 0);

  updateMotors();

  updateControlBar();

  checkScrolling();


}


