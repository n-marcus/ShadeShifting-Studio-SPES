#include <EEPROM.h>
#define NUM_SCENES 2
#define NUM_MOTORS 25
#define EEPROM_SIZE 4000

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
// Create an array of Scene structs to hold data for 25 scenes and 25 motors each

void setupScenes() {
  // Initialize data for each motor in each scene
  for (int i = 0; i < NUM_SCENES; ++i) {
    for (int j = 0; j < NUM_MOTORS; ++j) {
      scenes[i].motors[j].mode = round(random(1) + 0.5);
      scenes[i].motors[j].value = random(36000) / 100.;  // Set initial value to 0
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
  Serial.println("Total size needed is " + String(sizeof(MotorData) * (NUM_SCENES * NUM_MOTORS)));

  for (int sceneIdx = 0; sceneIdx < NUM_SCENES; ++sceneIdx) {
    for (int motorIdx = 0; motorIdx < NUM_MOTORS; ++motorIdx) {
      EEPROM.put(addr, scenes[sceneIdx].motors[motorIdx]);
      addr += sizeof(MotorData);
    }
  }
  EEPROM.commit();
  EEPROM.end();
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
void printMotorData() {
  for (int sceneIdx = 0; sceneIdx < NUM_SCENES; ++sceneIdx) {
    Serial.print("Scene ");
    Serial.print(sceneIdx);
    Serial.println(":");
    for (int motorIdx = 0; motorIdx < NUM_MOTORS; ++motorIdx) {
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
