int clickCount = 0;
int doubleClickDelay = 250; // Adjust this value as needed (in milliseconds)
int lastClickTime = 0;


int lastScrollTime = 0;
int scrollDelay = 500;
boolean scrolling = false;
boolean _scrolling = false;

void mouseReleased() {

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
