ControlBar controlBar;
int controlBarHeight = 150;
ControlP5 cp5;

class ControlBar {
  float xPos, yPos, barWidth, barHeight;
  float bottomPadding = 10;
  ControlBar(float _xPos, float _yPos, float _barWidth, float _barHeight) {
    xPos = _xPos;
    yPos = _yPos;
    barWidth = _barWidth;
    barHeight = _barHeight;


    //Create a button
    cp5.addButton("resetAllToHomeButton")
      .setPosition(50, (barHeight - bottomPadding) * 0.75)
      .setSize(100, 30)
      .setLabel("Reset all to home");

    cp5.addButton("printScene")
      .setPosition(150, (barHeight - bottomPadding) * 0.75)
      .setSize(100, 30)
      .setLabel("Print scene");
  }


  void update() {
    //title
    textSize(32);
    text("Shade Shifting", 50, 50);



    //bottom seperator line
    strokeWeight(1);
    stroke(255);
    line(xPos, yPos + barHeight - bottomPadding, xPos + barWidth, yPos + barHeight - bottomPadding);
  }
}

void setupControlBar() {
  cp5 = new ControlP5(this);
  controlBar = new ControlBar(0, 0, width, controlBarHeight);
}
void resetAllToHomeButton() {
  println("Reset all to home pressed");
  resetAllMotorsToHome();
  sendResetToHomeOSC();
}


void updateControlBar() {
  controlBar.update();
}
