/* autogenerated by Processing revision 1293 on 2023-12-13 */
import processing.core.*;
import processing.data.*;
import processing.event.*;
import processing.opengl.*;

import netP5.*;
import oscP5.*;
import controlP5.*;

import java.util.HashMap;
import java.util.ArrayList;
import java.io.File;
import java.io.BufferedReader;
import java.io.PrintWriter;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.IOException;

public class ProcessingControlBoard extends PApplet {








public void setup() {
  /* size commented out by preprocessor */;
  /* pixelDensity commented out by preprocessor */;
  /* smooth commented out by preprocessor */;
  setupOSC();
  setupControlBar();
  setupMotors(0, controlBarHeight, width, height - controlBarHeight );
}

public void draw() {
  background(0, 0, 0);

  updateMotors();

  updateControlBar();

  checkScrolling();


}


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
      .setPosition(50, (barHeight - bottomPadding) * 0.75f)
      .setSize(80, 30)
      .setLabel("Reset to home");
  }


  public void update() {
    //title
    textSize(32);
    text("Shade Shifting", 50, 50);



    //bottom seperator line
    strokeWeight(1);
    stroke(255);
    line(xPos, yPos + barHeight - bottomPadding, xPos + barWidth, yPos + barHeight - bottomPadding);
  }
}

public void setupControlBar() {
  cp5 = new ControlP5(this);
  controlBar = new ControlBar(0, 0, width, controlBarHeight);
}
public void resetAllToHomeButton() {
  println("Reset all to home pressed");
  resetAllMotorsToHome();
}


public void updateControlBar() {
  controlBar.update();
}
ArrayList<Motor> motors = new ArrayList<Motor>();
int numMotors = 25;

class Motor {
    int index, x, y, w, h;
    float angle;
    
    //mode 0 = static, 1 = clockwise, 2 = counter clockwise;
    String[] modeNames = {"Static", "Clockwise", "Counter clockwise"};
    int mode = 0;
    
    int borderColorBase = 100;
    int borderColor = borderColorBase;
    
    int maxSpeed = 5;
    
    float speed = 1;
    
    boolean movingHome = false;
    
    boolean scrolling = false;
    boolean _scrolling = false;
    
    Motor(int _index, float _x, float _y) {
        index = _index;
        x = PApplet.parseInt(_x);
        y = PApplet.parseInt(_y);
        w = 100;
        h = 100;
        
        println("Created motor " + index + " at " + x + ", " + y);
}
    
    public void update() {
        updateRotation();
        drawMotor();
}
    
    public void updateRotation() {
        if (mode == 1) {
            angle += speed;
            angle %= 360;
        }
}
    
    public void drawMotor() {
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
        text(index, w / 2, h * 0.1f);
        
        text(modeNames[mode], w / 2, h * 0.2f);
        
        
        if (mode == 0) {
            text("Angle:" + nf(angle, 0, 2), w / 2, h * 0.9f);
        } else {
            text("Speed:" + nf(speed, 0, 2), w / 2, h * 0.9f);
        }
        
        translate(w / 2, h / 2);
        rotate(radians(angle));
        rectMode(CENTER);
        stroke(255);
        rect(0, 0, 5, h * 0.5f);
        
        pop();
}
    
    public void handleScroll(float scrollSpeed) {
            scrolling = true;
        if (scrollSpeed != 0) {
            println("Really scrolling");
        } 
        
        if (mode == 0) {
            angle += scrollSpeed;
            angle = angle % 360;
            
            sendMotorAngleOSC();
        } else {
            speed += scrollSpeed * 0.1f;
            speed = max( -maxSpeed, min(speed, maxSpeed));
        }
}
    
    public void resetPosition() {
        angle = 0;
        mode = 0;
}
    
    public void handleClick() {
        borderColor = 255;
        mode++;
        mode %= 2;
}
    
    public void resetToHome() {
        angle = 0;
        mode = 0;
}
    
    public void sendMotorAngleOSC() {
        OscMessage myMessage = new OscMessage("/moveToAngle");
        
        myMessage.add(index); /* add an int to the osc message */
        myMessage.add(PApplet.parseInt(angle));
        oscP5.send(myMessage, myRemoteLocation);
}
        
        public void sendMotorSpeedOSC() {
        OscMessage myMessage = new OscMessage("/setSpeedAndDirection");
        
        myMessage.add(index); /* add an int to the osc message */
        myMessage.add(speed);
        oscP5.send(myMessage, myRemoteLocation);
}
}

public void updateMotors() {
    for (int i = 0; i < numMotors; i ++) {
        Motor _motor = motors.get(i);
        _motor.update();
}
}

public void setupMotors(float gridXPos, float gridYPos, float gridWidth, float gridHeight) {
    //make a grid of motors
    for (int i = 0; i < numMotors; i++) {
        //calculate an x position
        float xPos = ((i % 5) * gridWidth / 5);
        xPos+= (gridWidth / 5) / 4;
        xPos+= gridXPos;
        
        //calculate an y position
        float yPos = floor(i / 5) * (gridHeight * 0.2f);
        yPos+= gridYPos;
        
        //create themotor class
        motors.add(new Motor(i, xPos, yPos));
}
}

public void checkMotorsForScroll(float scrollSpeed) {
    for (int i = 0; i < numMotors; i ++) {
        
        Motor _motor = motors.get(i);
        if (mouseX > _motor.x && mouseX <  _motor.x +  _motor.w && mouseY >  _motor.y && mouseY <  _motor.y +  _motor.h) {
            _motor.handleScroll(scrollSpeed);
        }
}
}

public void checkMotorsForClick() {
    for (int i = 0; i < numMotors; i ++) {
        Motor _motor = motors.get(i);
        if (mouseX > _motor.x &&  mouseX <  _motor.x +  _motor.w && mouseY >  _motor.y && mouseY <  _motor.y +  _motor.h) {
            _motor.handleClick();
        }
}
}

public void resetMotorAtMouse() {
    for (int i = 0; i < numMotors; i ++) {
        Motor _motor = motors.get(i);
        if (mouseX > _motor.x &&  mouseX <  _motor.x +  _motor.w && mouseY >  _motor.y && mouseY <  _motor.y +  _motor.h) {
            _motor.resetPosition();
        }
}
}

public void resetAllMotorsToHome() {
    for (int i = 0; i < numMotors; i ++) {
        Motor _motor = motors.get(i);
        _motor.resetToHome();
}
}
int clickCount = 0;
int doubleClickDelay = 250; // Adjust this value as needed (in milliseconds)
int lastClickTime = 0;


int lastScrollTime = 0;
int scrollDelay = 500; 
boolean scrolling = false;

public void mouseReleased() {
  
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

public void checkScrolling() { 
    if (millis() - lastScrollTime > scrollDelay) {
    // Scrolling stopped
    println("Scrolling stopped");
    scrolling = false;
    // Send your message or perform any action here
  }
}

public void mouseWheel(MouseEvent event) {
  lastScrollTime = millis();
  scrolling = true;
  float scrollSpeed = event.getCount();
  checkMotorsForScroll(scrollSpeed * 0.5f);
}
OscP5 oscP5;
NetAddress myRemoteLocation;

public void setupOSC() { 
    oscP5 = new OscP5(this,12001);
    myRemoteLocation = new NetAddress("255.255.255.255",8888);
}

public void mouseDragged() { 
    
    OscMessage myMessage = new OscMessage("/test");
    
    myMessage.add(mouseX); /* add an int to the osc message */
    oscP5.send(myMessage, myRemoteLocation); 
    
    println("I have send a message " + mouseX );
}


  public void settings() { size(800, 800);
pixelDensity(2);
smooth(); }

  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "ProcessingControlBoard" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}