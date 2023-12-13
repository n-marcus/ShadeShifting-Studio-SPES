import java.util.ArrayList;
import java.util.HashMap;
import processing.data.JSONObject;
ArrayList<HashMap<String, MotorSceneData>> scenes = new ArrayList<>();

int numScenes =25;

int currentScene = 0;

// Define the structure for a motor
class MotorSceneData {
  String mode;
  float value;

  MotorSceneData(String mode, int value) {
    this.mode = mode;
    this.value = value;
  }
}

void setupSceneHashMap() {
  for (int s = 0; s < numScenes; s++) {
    HashMap<String, MotorSceneData> scene = new HashMap<>();
    for (int i = 0; i < numMotors; i++) {
      scene.put("Motor" + i, new MotorSceneData("angle", 0)); // Create new MotorSceneData instances
    }
    scenes.add(scene);
  }

  //print the current scene
  printScene();
}



void printScene() {
  println("Scene dump " + currentScene);
  for (int i = 0; i < numMotors; i ++) {
    // Accessing data from tables
    MotorSceneData motor = scenes.get(currentScene).get("Motor" + i);
    println("  Motor " + i + ": " + motor.mode + " " +  motor.value);
  }
}

void editMotorData(int sceneIndex, int motorIndex, String newMode, float newValue) {
  //first compose the name of this motor in the scene hash map
  String motorKey = "Motor" + motorIndex;

  //check if this is a valid scene
  if (sceneIndex >= 0 && sceneIndex < scenes.size()) {
    //retreive the specific scene
    HashMap<String, MotorSceneData> scene = scenes.get(sceneIndex);

    //println(scene);
    //check if the scene has this motor
    if (scene.containsKey(motorKey)) {
      //make a local reference to the motor
      MotorSceneData motor = scene.get(motorKey);

      //update the values
      motor.mode = newMode;
      motor.value = newValue;
      println("Updated Mode of " + motorKey + " in Scene " + sceneIndex + ": " + motor.mode + " " + motor.value);
    } else {
      println("Motor key not found in the scene.");
    }
  } else {
    println("Scene index out of bounds.");
  }
}

void setScene(int sceneIndex) {
  //get the scene info and set it into the motors
  HashMap<String, MotorSceneData> scene = scenes.get(sceneIndex);
  println("Setting motors to scene " + sceneIndex);

  //loop over all motors
  for (int i = 0; i < numMotors; i ++) {
    //unique name for this motor
    String motorKey = "Motor" + i;

    //check if the scene has this motor
    if (scene.containsKey(motorKey)) {
      //make a local reference to the motor and the data for the motor in the scene
      MotorSceneData motorData = scene.get(motorKey);
      Motor _motor = motors.get(i);

      //check which mode this motor should be in
      //and set the appropriate values
      if (motorData.mode == "angle") {
        _motor.angle = motorData.value;
        _motor.mode = 0;
      } else if (motorData.mode == "speed") {
        _motor.speed = motorData.value / 100.;
        _motor.mode = 1;
      }
      
      //tell the motor to send its new state over osc 
      _motor.sendOSCUpdate();
    }
  }
}

void decrementScene() {

  currentScene --;
  if (currentScene < 0) {
    currentScene = numScenes - 1;
  }
  println("Decrementing scene " + currentScene);

  setScene(currentScene);
}

void incrementScene() {

  currentScene ++;
  if (currentScene > numScenes) {
    currentScene = 0;
  }
  println("Incrementing scene "  + currentScene );
  setScene(currentScene);
}


void saveScenesToFile() {
  println("Saving scene to JSON");
  JSONObject json = new JSONObject();

  for (int i = 0; i < scenes.size(); i++) {
    JSONObject sceneJSON = new JSONObject();
    HashMap<String, MotorSceneData> scene = scenes.get(i);

    for (String motorKey : scene.keySet()) {
      MotorSceneData motor = scene.get(motorKey);
      JSONObject motorJSON = new JSONObject();
      motorJSON.setString("mode", motor.mode);
      motorJSON.setFloat("value", motor.value);
      sceneJSON.setJSONObject(motorKey, motorJSON);
    }
    json.setJSONObject("scene" + i, sceneJSON);
  }

  saveJSONObject(json, "scenes.json");
}
