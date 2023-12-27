#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <elapsedMillis.h>

// Set WiFi credentials
#define WIFI_SSID "ShadeShifting"
#define WIFI_PASS "FridayDance2020!"
#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#endif

#define NUM_SCENES 15
#define NUM_MOTORS 25
// Define a struct for motor data
struct MotorData {
  int mode;  // Mode can be 0 for angle or 1 for speed
  float value;
};

// Define a struct for a scene containing motor data
struct Scene {
  MotorData motors[NUM_MOTORS];
};

Scene scenes[NUM_SCENES];
int currentScene = 0;
// Create an array of Scene structs to hold data for 15 scenes and 25 motors each

WiFiUDP Udp;
const IPAddress outIp(255, 255, 255, 255);  // remote IP (not needed for receive)
const unsigned int outPort = 8888;
const unsigned int localPort = 8001;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupWifi();
  setupScenes();
  // saveScenesToEEPROM();
  // loadScenesFromEEPROM();
  // printMotorData();

  sendResetCommand();
}

void loop() {
  checkOSC();
  checkScene();
  checkSerial();
}
