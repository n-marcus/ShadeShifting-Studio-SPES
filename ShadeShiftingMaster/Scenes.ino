#include <EEPROM.h>


// Define a struct for motor data
struct MotorData {
  int mode;  // Mode can be 0 for angle or 1 for speed
  float value;
};

// Define a struct for a scene containing motor data
struct Scene {
  MotorData motors[25];
};

// Create an array of Scene structs to hold data for 25 scenes and 25 motors each
Scene scenes[25];

void setupScenes() {
  // Initialize data for each motor in each scene
  for (int i = 0; i < 25; ++i) {
    for (int j = 0; j < 25; ++j) {
      scenes[i].motors[j].mode = round(random());  
      scenes[i].motors[j].value = random(360);              // Set initial value to 0
    }
  }
}

// Function to save scenes object to EEPROM
void saveScenesToEEPROM() {
  int addr = 0; // Start address in EEPROM

  for (int sceneIdx = 0; sceneIdx < 25; ++sceneIdx) {
    for (int motorIdx = 0; motorIdx < 25; ++motorIdx) {
      EEPROM.put(addr, scenes[sceneIdx].motors[motorIdx]);
      addr += sizeof(MotorData);
    }
  }
}

void loadScenesFromEEPROM() {
  int addr = 0; // Start address in EEPROM

  for (int sceneIdx = 0; sceneIdx < 25; ++sceneIdx) {
    for (int motorIdx = 0; motorIdx < 25; ++motorIdx) {
      EEPROM.get(addr, scenes[sceneIdx].motors[motorIdx]);
      addr += sizeof(MotorData);
    }
  }
}

// Function to print mode and value for each motor in scenes
void printMotorData() {
  for (int sceneIdx = 0; sceneIdx < 25; ++sceneIdx) {
    Serial.print("Scene ");
    Serial.print(sceneIdx);
    Serial.println(":");
    for (int motorIdx = 0; motorIdx < 25; ++motorIdx) {
      Serial.print("  Scene " + String(sceneIdx) + "Motor ");
      Serial.print(motorIdx);
      Serial.print(" - Mode: ");
      Serial.print(scenes[sceneIdx].motors[motorIdx].mode);
      Serial.print(", Value: ");
      Serial.println(scenes[sceneIdx].motors[motorIdx].value);
    }
    Serial.println();
  }
}
