import netP5.*;
import oscP5.*;

void setup() { 
    size(800,800);
    pixelDensity(2);
    smooth();
    setupOSC();
    setupMotors(0, controlBarHeight , width, height - controlBarHeight );

    controlBar = new ControlBar(0,0,width, controlBarHeight );
   
}

void draw() { 
    background(0,0,0);

    updateMotors();

    controlBar.update();
}

void mouseWheel(MouseEvent event) {
  float scrollSpeed = event.getCount();
  checkMotorsForScroll(scrollSpeed * 0.5);
}
