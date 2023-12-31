#include <EEPROM.h>
#define EEPROM_SIZE 4000



void setupScenes() {
  // Initialize data for each motor in each scene
  for (int i = 0; i < NUM_SCENES; ++i) {
    for (int j = 0; j < NUM_MOTORS; ++j) {
      // scenes[i].motors[j].mode = random(2);
      // scenes[i].motors[j].value = random(36000) / 100.;  // Set initial value to 0
      scenes[i].motors[j].mode = 0;
      scenes[i].motors[j].value = 0;  // Set initial value to 0
    }
  }
}

void checkActiveMotors() {
  //this function checks how many motors have send a heartbeat in the last 6 seconds or so
  int maxTimeOut = 6000;
  long now = millis();
  numActiveMotors = 0;
  for (int motor = 0; motor < NUM_MOTORS; motor++) {
    if (now - lastMotorHeartBeatMs[motor] < maxTimeOut) {
      //if this motor has send a heartbeat within hte last 6 seconds, count it
      numActiveMotors++;
    }
  }
}

// Function to save scenes object to EEPROM
void saveScenesToEEPROM() {
  //Init EEPROM
  EEPROM.begin(EEPROM_SIZE);
  int addr = 0;  // Start address in EEPROM
  Serial.println("Saving scenes to EEPROM");
  Serial.println("Size per scene is " + String(sizeof(MotorData)));
  int totalSizeNeeded = sizeof(MotorData) * (NUM_SCENES * NUM_MOTORS);
  Serial.println("Total size needed is " + String(totalSizeNeeded));
  Serial.println("Total size available is " + String(EEPROM_SIZE));

  if (totalSizeNeeded > EEPROM_SIZE) {
    Serial.println("EEPROM is nog big enough to store this data, aborting");
    return;
  }

  for (int sceneIdx = 0; sceneIdx < NUM_SCENES; ++sceneIdx) {
    for (int motorIdx = 0; motorIdx < NUM_MOTORS; ++motorIdx) {
      EEPROM.put(addr, scenes[sceneIdx].motors[motorIdx]);
      addr += sizeof(MotorData);
    }
  }
  EEPROM.commit();
  EEPROM.end();

  Serial.println("Finished saving to EEPROM");
}

void loadScenesFromEEPROM() {
  int addr = 0;  // Start address in EEPROM
    //Init EEPROM
  EEPROM.begin(EEPROM_SIZE);

  for (int sceneIdx = 0; sceneIdx < NUM_SCENES; ++sceneIdx) {
    for (int motorIdx = 0; motorIdx < NUM_MOTORS; ++motorIdx) {
      EEPROM.get(addr, scenes[sceneIdx].motors[motorIdx]);
      addr += sizeof(MotorData);
    }
  }
  EEPROM.end();
}

// Function to print mode and value for each motor in scenes
void printMotorData(bool all, int scene) {
  if (all) {

    for (int sceneIdx = 0; sceneIdx < NUM_SCENES; ++sceneIdx) {
      Serial.print("Scene ");
      Serial.print(sceneIdx);
      Serial.println(":");
      for (int motorIdx = 0; motorIdx < NUM_MOTORS; ++motorIdx) {
        Serial.print("  Scene " + String(sceneIdx) + " - Motor ");
        Serial.print(motorIdx);
        Serial.print(" - Mode: ");
        Serial.print(scenes[sceneIdx].motors[motorIdx].mode);
        Serial.print(", Value: ");
        Serial.println(scenes[sceneIdx].motors[motorIdx].value);
      }
      Serial.println();
    }
  } else {
    DEBUG_PRINT("Data from scene " + String(scene));
    int sceneIdx = scene;
    for (int motorIdx = 0; motorIdx < NUM_MOTORS; ++motorIdx) {
      Serial.print("  Scene " + String(sceneIdx) + " - Motor ");
      Serial.print(motorIdx);
      Serial.print(" - Mode: ");
      Serial.print(scenes[sceneIdx].motors[motorIdx].mode);
      Serial.print(", Value: ");
      Serial.println(scenes[sceneIdx].motors[motorIdx].value);
    }
    Serial.println();
  }
}


void checkScene() {
  if (!isPlaying) {
    //if we are not playing
    // dont do anything
    return;
  }

  //if we are playing, check if we are resetting
  if (sceneMode == 1) {
    //if we are actively in a scene (not resetting)
    if (timeInCurrentScene > TIME_PER_SCENE_MS) {
      DEBUG_PRINT("Have been in scene " + String(currentScene) + " for too long now, moving on...");
      goToNextScene();
    }
  } else {
    //if we are resetting
    if (timeResetting > RESET_TIMEOUT) {
      //if we have been waiting for too long
      DEBUG_PRINT("We have been resetting for too long, set the current scene to be active");
      setSceneActive();


    } else {
      // DEBUG_PRINT("Waiting for all nodes to be resetted");
    }
  }
}

void setSceneActive() {
  sceneMode = 1;
  DEBUG_PRINT("Set scene " + String(currentScene) + " to active");

  timeInCurrentScene = 0;
  for (int motor = 0; motor < NUM_MOTORS; motor++) {
    sendDataForMotorInScene(currentScene, motor);
  }

  setLCDCurrentScene();
}

void sendResetCommand() {
  DEBUG_PRINT("Sending reset command to all nodes");
  //say we are now in reset mode
  sceneMode = 0;


  Serial.println("Now sending all to move to 0 degrees, then waiting a bit");
  //first we set all the motors to 0 degrees
  for (int i = 0; i < NUM_MOTORS; i++) {
    OSCMessage msg("/moveToAngle");
    msg.add(i);
    msg.add(0.0);
    Udp.beginPacket(outIp, outPort);

    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
  }

  //wait a while to have them move to 0
  delay(5000);

  Serial.println("Now sending home message");
  for (int i = 0; i < NUM_MOTORS; i++) {
    for (int bangMeHarder = 0; bangMeHarder < 3; bangMeHarder++) {

      //send an OSC message to all nodes
      OSCMessage msg("/home");
      msg.add(i);
      Udp.beginPacket(outIp, outPort);

      msg.send(Udp);
      Udp.endPacket();
      msg.empty();
      delay(1);
    }
  }

  timeResetting = 0;

  setMotorsResetted();
}

void goToNextScene() {
  //this function will move us to the next scene
  currentScene++;
  if (currentScene >= NUM_SCENES) {
    currentScene = 0;
  }

  setLCDResetting();

  sendResetCommand();
}

void setMotorsResetted() {
  for (int i = 0; i < NUM_MOTORS; ++i) {
    motorsResetted[i] = false;
  }
}

void pause() {
  isPlaying = false;

  sendResetCommand();
}

void start() {
  timeResetting = RESET_TIMEOUT;
  timeInCurrentScene = 0;
  isPlaying = true;
  // sendResetCommand();
}