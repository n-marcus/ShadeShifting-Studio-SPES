

// Set WiFi credentials
#define WIFI_SSID "ShadeShifting"
#define WIFI_PASS "FridayDance2020!"

//HIER KAN JE INSTELLEN HOELANG EEN SCENE DUURT
#define TIME_PER_SCENE_MS 20 * 1000

//DITI S HOE LANG IE WACHT TOT ALLE NODES ZIJN GERESETTED
#define RESET_TIMEOUT 20 * 1000

//COMMENT DEZE REGEL WEG OM DEBUG MODE UIT TE ZETTEN
#define DEBUG



///DONT EDIIT BELOW THIS
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <OSCBundle.h>
#include <OSCData.h>
#include <elapsedMillis.h>
#include <Button.h>
#include "Adafruit_Debounce.h"

#include <FastLED.h>

elapsedMillis timeInCurrentScene;
elapsedMillis timeResetting;


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
int _currentScene = 0;
bool isPlaying = false;

int sceneMode = 0;  //0 means resetting, 1 means active

bool motorsResetted[NUM_MOTORS];

long lastMotorHeartBeatMs[NUM_MOTORS];
int numActiveMotors = 0;

// Create an array of Scene structs to hold data for 15 scenes and 25 motors each

WiFiUDP Udp;
const IPAddress outIp(255, 255, 255, 255);  // remote IP (not needed for receive)
const unsigned int outPort = 8888;
const unsigned int localPort = 8001;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setupLCD();
  setupWifi();
  setupScenes();
  setupLED();
  setupButton();
  // saveScenesToEEPROM();
  loadScenesFromEEPROM();
  // printMotorData();
  sendResetCommand();

  start();
}

void loop() {
  checkOSC();
  checkScene();
  checkSerial();
  checkLCD();
  checkLED();
  checkButton();
}
