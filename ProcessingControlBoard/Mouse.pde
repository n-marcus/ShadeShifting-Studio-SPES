int lastScrollTime = 0;
int scrollDelay = 500;
boolean scrolling = false;
boolean _scrolling = false;


void mouseReleased() {
  if (mouseButton == LEFT) {
    println("Left-click");
    checkMotorsForClick();
  } else if (mouseButton == RIGHT && keyPressed && key == CODED && keyCode == SHIFT) {
    // Right-click while holding Shift
    println("Right-click with Shift");
    homeMotorAtMouse();
    // Add your action for right-click with Shift here
  } else if (mouseButton == RIGHT) {
    // Regular right-click action
    println("Right-click");
    resetMotorAtMouse();
  }
}

void checkScrolling() {
  if (millis() - lastScrollTime > scrollDelay) {
    // Scrolling stopped
    scrolling = false;
  }

  if (scrolling == false && scrolling != _scrolling) {
    println("Scrolling stopped");
    sendMotorsScrollEndedEvent();
  }

  _scrolling = scrolling;
}

void mouseWheel(MouseEvent event) {
  lastScrollTime = millis();
  scrolling = true;
  float scrollSpeed = event.getCount();
  checkMotorsForScroll(scrollSpeed * 0.5);
}
