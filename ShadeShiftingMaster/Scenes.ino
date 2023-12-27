#include <EEPROM.h>
#define EEPROM_SIZE 4000


int sceneMode = 0;  //0 means resetting, 1 means active

elapsedMillis timeInCurrentScene;
elapsedMillis timeResetting;

bool isPlaying = false;

void setupScenes() {
  // Initialize data for each motor in each scene
  for (int i = 0; i < NUM_SCENES; ++i) {
    for (int j = 0; j < NUM_MOTORS; ++j) {
      scenes[i].motors[j].mode = random(2);
      scenes[i].motors[j].value = random(36000) / 100.;  // Set initial value to 0
      scenes[i].motors[j].mode = 0;
      scenes[i].motors[j].value = 0;  // Set initial value to 0
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
void printMotorData(bool all, int scene ) {
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
}

void sendResetCommand() {
  DEBUG_PRINT("Sending reset command to all nodes");
  //say we are now in reset mode
  sceneMode = 0;

  for (int i = 0; i < NUM_MOTORS; i++) {

    //send an OSC message to all nodes
    OSCMessage msg("/home");
    msg.add(i);
    Udp.beginPacket(outIp, outPort);

    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
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

  sendResetCommand();
}

void setMotorsResetted() {
  for (int i = 0; i < NUM_MOTORS; ++i) {
    motorsResetted[i] = false;
  }
}

void pause() {
  isPlaying = false;

}

void start() { 
  isPlaying = true;
  sendResetCommand();
}