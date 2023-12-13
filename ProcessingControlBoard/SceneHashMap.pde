import java.util.ArrayList;
import java.util.HashMap;
ArrayList<HashMap<String, MotorSceneData>> scenes = new ArrayList<>();

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
  HashMap<String, MotorSceneData> table1 = new HashMap<>();
  for (int i = 0; i < 25; i ++) {
    table1.put("Motor" + i, new MotorSceneData("angle", 0));
    scenes.add(table1);
  }

  //print the current scene
  printScene();
}

void printScene() {
  println("Scene dump " + currentScene);
  for (int i = 0; i < 25; i ++) {
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
    //check if the scene has this motor
    if (scene.containsKey(motorKey)) {
      //make a local reference to the motor
      MotorSceneData motor = scene.get(motorKey);

      //update the values
      motor.mode = newMode;
      motor.value = newValue;
      println("Updated Mode of Motor " + motorKey + " in Scene " + sceneIndex + ": " + motor.mode + " " + motor.value);
    } else {
      println("Motor key not found in the scene.");
    }
  } else {
    println("Scene index out of bounds.");
  }
}
