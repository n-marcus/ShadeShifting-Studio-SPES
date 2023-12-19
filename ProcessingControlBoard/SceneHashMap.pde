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


  //  scenes = loadScenesFromFile();
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
      if (sceneIndex == 0) {
        println("Updated Mode of " + motorKey + " in Scene " + sceneIndex + ": " + motor.mode + " " + motor.value);
        println(motor.mode, motor.value);
      }
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
      //println("Set scene thinks motor " + i + " should be set to " + motorData.mode + " " + motorData.value);
      //println("is angle " + (motorData.mode == "angle"));
      if (motorData.mode.equals("angle")) {
        _motor.angle = motorData.value;
        _motor.mode = 0;
      } else if (motorData.mode.equals("speed")) {
        _motor.speed = motorData.value / 100.;
        _motor.mode = 1;
      }
      
      //println("Set scene set Motor " + i + " to " + _motor.mode + " " + _motor.angle + " " + _motor.speed);

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

  for (int i = scenes.size() - 1; i >= 0; i--) {
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

void fileSelected(File selection) {
  if (selection == null) {
    println("File selection was canceled");
  } else {
    String filePath = selection.getAbsolutePath();
    loadScenesFromFile(filePath); // Pass the selected file path to load the JSON
  }
}

void loadScenesFromFile(String filePath) {
  JSONObject json = loadJSONObject(filePath); // Load the selected JSON file
  if (json == null) {
    println("Error: Could not load JSON file.");
    return;
  }

  for (int scene = 0; scene < numScenes; scene++) {
    String sceneKey = "scene" + scene;
    println("Loading scene " + scene);
    if (json.hasKey(sceneKey)) {
      JSONObject sceneJSON = json.getJSONObject(sceneKey);
      if (sceneJSON != null) {
        //HashMap<String, MotorSceneData> scene = scenes.get(scene);
        //print(sceneJSON.keys());

        for (int motor = 0; motor < sceneJSON.keys().size(); motor++ ) {
          String motorString = "Motor" + motor;

          //println(sceneJSON.get(motorString).getClass());

          //this is the object that contains the info for this motor in this scene
          JSONObject motorObject = sceneJSON.getJSONObject(motorString);

          //println(motorObject);
          String motorMode = motorObject.getString("mode");
          float motorValue = motorObject.getFloat("value");

          if (scene == 0) {
            println("Scene " + scene+ ": motor" + motor + ": ");
            println("Mode: " + motorMode + " ,value: " + motorValue);
          }
          //save to local memory
          editMotorData(scene, motor, motorMode, motorValue);
        }
      }
    }
  }

  //set all motors to the current scene in the new hasmap
  setScene(currentScene);
  printScene();
  
}
