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
      .setSize(90, 30)
      .setLabel("Reset all to home");

    cp5.addButton("printScene")
      .setPosition(150, (barHeight - bottomPadding) * 0.75)
      .setSize(90, 30)
      .setLabel("Print scene");

    cp5.addButton("decrementScene")
      .setPosition(250, (barHeight - bottomPadding) * 0.75)
      .setSize(30, 30)
      .setLabel("-");

    cp5.addButton("incrementScene")
      .setPosition(350, (barHeight - bottomPadding) * 0.75)
      .setSize(30, 30)
      .setLabel("+");

    cp5.addButton("saveScenesButton")
      .setPosition(400, (barHeight - bottomPadding) * 0.75)
      .setSize(90, 30)
      .setLabel("Save scenes");

    cp5.addButton("loadScenesButton")
      .setPosition(500, (barHeight - bottomPadding) * 0.75)
      .setSize(90, 30)
      .setLabel("Load scenes");

    cp5.addButton("saveScenesToMaster")
      .setPosition(600, (barHeight - bottomPadding) * 0.75)
      .setSize(90, 30)
      .setLabel("Scenes to master");
  }


  void update() {
    //title
    textSize(32);
    textAlign(CORNER, CORNER);
    text("Shade Shifting", 50, 50);


    textSize(18);
    textAlign(CENTER, BOTTOM);
    text(currentScene, 315, (barHeight - bottomPadding) * 0.925);


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

void loadScenesButton() {
  println("Load scenes button pressed");
  selectInput("Select JSON file:", "fileSelected");
}

void saveScenesButton() {
  println("Save scene button pressed");
  saveScenesToFile();
}
