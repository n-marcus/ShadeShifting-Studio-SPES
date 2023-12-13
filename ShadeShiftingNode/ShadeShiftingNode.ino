#include <AccelStepper.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <elapsedMillis.h>

//EDIT STUFF HERE
#define NODE_NUMBER 1
#define DEFAULT_ACCELERATION 500
float speedAcceleration = 0.1;

// Set WiFi credentials
#define WIFI_SSID "ShadeShifting"
#define WIFI_PASS "FridayDance2020!"

//STOP EDITING STUFF BELOW HERE

elapsedMillis sinceOSC;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP Udp;
// const IPAddress outIp(10,40,10,105);        // remote IP (not needed for receive)
// const unsigned int outPort = 9999;          // remote port (not needed for receive)
const unsigned int localPort = 8888;  // local port to listen for UDP packets (here's where we send the packets)

OSCErrorCode error;

#define stepsNeededForCircle 4800
#define stepsPerDegree 13.3

const int stp = 12;
const int dir = 13;
const int en = 14;
int HALSensorPin = 16;
// EG X-Y position bed driven by 2 steppers
// Alas its not possible to build an array of these with different pins for each 🙁
AccelStepper stepper(AccelStepper::DRIVER, stp, dir);

int loopIndex = 0;

bool motorRunning = true;
bool motorRunningSpeed = false;

int targetSpeed = 0;
float currentSpeed = 0;


void setup() {
  // Change these to suit your stepper if you want
  pinMode(HALSensorPin, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  setupSerial();

  setupWifi();

  stepper.setMaxSpeed(2500);

  homing();
}

void loop() {
  checkSerial();
  checkOSC();
  checkLED();
  runMotor();
}

