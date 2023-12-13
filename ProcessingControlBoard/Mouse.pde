int clickCount = 0;
int doubleClickDelay = 250; // Adjust this value as needed (in milliseconds)
int lastClickTime = 0;

void mouseClicked() {
  if (millis() - lastClickTime < doubleClickDelay) {
    clickCount++;
  } else {
    clickCount = 1;
  }

  lastClickTime = millis();

  if (clickCount == 1) {
    // Single click action
    println("Single click");
    checkMotorsForClick();
  } else if (clickCount == 2) {
    // Double click action
    println("Double click");
    resetMotorAtMouse();
    clickCount = 0; // Reset click count for the next set of clicks
  }
}