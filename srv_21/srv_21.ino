/*****************************************************
    Surface Reconnaissance Vehicle program
    by Kristijan Segulja
    Program to control robotic vehicle with IR remote controller
    This is meant to simulate the SRV mechanics from
    Elite Dangerous - video game developed by Frontier
    Developments

    Current features:
    - Feature: receive IR data
    - Feature: light intensity measurement
    - Feature: status lights
    - Feature: front lights
    - Feature: break (stop) lights
    - Feature: ultrasonic sensor obstacle detector
    - Feature: gyro sensor speed regulator
    - Feature: line detection
    - Operational Mode 1: drive assist
    - Operational Mode 2: manual drive
    - Operational Mode 3: line following

    Features to be added:
    -

*****************************************************/

#include "BlinkTest.h"
#include "MotorControl.h"
#include "Sensors.h"
#include "LEDs.h"

MotorControl motorControl;
BlinkTest blinkTest;
Sensors sensors;
LEDs LED;

// Motor A connections
int enA = 12;
int inA1 = 11;
int inA2 = 10;

// Motor B connections
int enB = 8;
int inB1 = 7;
int inB2 = 6;

// Sensor Pins
int photoresistorPin = 28;
int trigPin = 19;
int echoPin = 18;
// irDecoder is 17. It is defined in Sensors.h

// Reflectance sensors array
int reflectancePin1 = 26;
int reflectancePin2 = 27;

// Control
String forward = "2";
String backward = "8";
String left = "4";
String right = "6";
String brake = "5";
String driveAssistToggle = "U/SD";
String modeSelect = "MODE";
String frontLightON = "+";
String frontLightOFF = "-";
bool driveAssist = true;
int lightTreshold = 800;

bool modeSelection = false;
String mode = "0"; // Default mode is 0 -> Mode selection

// LEDs
int stateLED[3] = {20, 21, 22}; // State LED (R,G,B)
int frontLED = 15;
int backLED = 14;


void setup() {
  delay(2000);
  Serial.begin(9600);
  Serial.println("Setup in progress");
  delay(1000);
  motorControl.SetupMotors(enA, enB, inA1, inA2, inB1, inB2);
  blinkTest.SetBlink();
  if (!sensors.SetupMPU(2, 250, 260)) {
    Serial.println("MPU Not found!");
  } else {
    Serial.println("MPU setup successfuly!");
  }
  LED.SetupLEDs(frontLED, backLED, stateLED[0], stateLED[1], stateLED[2]);
}
void loop() {
  // Enter 'mode selection' mode
  if (sensors.GetRemoteData() == "MODE") {
    mode = "0";
    motorControl.Stop();
  }

  // Switch modes depending on mode value
  if (mode == "0") { // MODE SELECT
    ModeSelect();
    LED.SetStateLight("WHITE");
  }
  else if (mode == "1") { // DRIVE ASSIST
    DriveAssist();
    LED.SetStateLight("BLUE");
  }
  else if (mode == "2") { // MANUAL DRIVE
    ManualDrive();
    LED.SetStateLight("GREEN");
  }
  else if (mode == "3") { // LINE FOLLOWING
    FollowLine();
    LED.SetStateLight("YELLOW");
  }
  else { // UNKOWN STATE, SET LED TO RED
    LED.SetStateLight("RED");
  }
  
}

// Function to test motors
void TestMotors() {
  // FORWARD
  motorControl.Drive(250, 250);
  delay(1000);
  motorControl.Stop();
  delay(500);
  // TURN LEFT
  motorControl.Drive(-250, 250);
  delay(1000);
  motorControl.Stop();
  delay(500);
  // TURN RIGHT
  motorControl.Drive(250, -250);
  delay(1000);
  motorControl.Stop();
  delay(500);
  // BACKWARD
  motorControl.Drive(-250, -250);
  delay(1000);
  motorControl.Stop();
  delay(500);
}

// Function to test all sensors
void TestSensors() {
  // Gather data
  String lightData = "Light: " + (String)sensors.ReadLightIntensity(photoresistorPin);
  String distanceData = "Distance: " + (String)sensors.ReadDistance(trigPin, echoPin);
  String gyroData = "Rotation: | X: " + (String)sensors.GetMPURotation("X") + " | Y: " + (String)sensors.GetMPURotation("Y") + " | Z: " + (String)sensors.GetMPURotation("Z");
  String accelerationData = "Acceleration: | X: " + (String)sensors.GetMPUAcceleration("X") + " | Y: " + (String)sensors.GetMPUAcceleration("Y") + " | Z: " + (String)sensors.GetMPUAcceleration("Z");
  String mpuTemperatureData = "Temperature: " + (String)sensors.GetMPUTemperature();
  String remoteData = "IR Controller Data: " + (String)sensors.GetRemoteData();
  String reflectanceData = "Reflectance: Left: " + (String)sensors.ReadReflectanceData(reflectancePin1) + ", Right: " + (String)sensors.ReadReflectanceData(reflectancePin2);

  // Print data
  Serial.println(lightData);
  Serial.println(distanceData);
  Serial.println(gyroData);
  Serial.println(accelerationData);
  Serial.println(mpuTemperatureData);
  Serial.println(remoteData);
  Serial.println(reflectanceData);
  Serial.println();
}

void ModeSelect() {
  if (sensors.irDataAvailable && sensors.keyIsHeld) {
    mode = sensors.GetRemoteData();
    Serial.print("Switching to mode: ");
    Serial.println(mode);
  }
}

// Function to control the vehicle while the drive assist is ON
void DriveAssist() {
  float speedModifier = abs(sensors.GetMPURotationAngle("Y") / 45); // Get the speed modifier based on the angle (45 is the angle where the power is set to maximum)

  // Adjust speed modifier so it does not fall out of boundaries
  if (speedModifier > 1) { // Get speed modifier back to 1 if greater than 1
    speedModifier = 1;
  }
  if (speedModifier < 0.7) { // Get speed modifier back to 0.7 in order to let the motors work with enough power if lesser than 0.7)
    speedModifier = 0.7;
  }

  // Get new control data
  String IRdata = sensors.GetRemoteData();
  float forwardDistance = sensors.ReadDistance(trigPin, echoPin);

  Serial.println(forwardDistance);
  if (forwardDistance <= 5 && IRdata != backward) {
    motorControl.Stop();
  } else {
    if (IRdata == forward && forwardDistance > 5) { // Forward
      motorControl.Drive(250 * speedModifier, 250 * speedModifier);
    }
    if (IRdata == backward) { // Backward
      motorControl.Drive(-250 * speedModifier, -250 * speedModifier);
    }
    if (IRdata == left) { // Left
      motorControl.Drive(-250 * speedModifier, 250 * speedModifier);
    }
    if (IRdata == right) { // Right
      motorControl.Drive(250 * speedModifier, -250 * speedModifier);
    }
  }
  if (IRdata == brake) { // Brake
    motorControl.Stop();
    LED.ToggleBackLights(true);
  } else {
    LED.ToggleBackLights(false);
  }

  if (sensors.ReadLightIntensity(28) > lightTreshold) {
    // TURN ON THE LIGHTS
    LED.ToggleFrontLights(true);
  } else {
    // TURN OFF THE LIGHTS
    LED.ToggleFrontLights(false);
  }
}

// Function to control the vehicle while the drive assist is OFF
void ManualDrive() {

  // Get new control data
  String IRdata = sensors.GetRemoteData();
  
  // Vehicle control
  if (IRdata == forward) { // Forward
    motorControl.Drive(250, 250);
  }
  if (IRdata == backward) { // Backward
    motorControl.Drive(-250, -250);
  }
  if (IRdata == left) { // Left
    motorControl.Drive(-250, 250);
  }
  if (IRdata == right) { // Right
    motorControl.Drive(250, -250);
  }
  if (IRdata == brake) { // Brake
    motorControl.Stop();
    LED.ToggleBackLights(true);
  } else {
    LED.ToggleBackLights(false);
  }

  // Front lights control
  if (IRdata == frontLightON) {
    LED.ToggleFrontLights(true);
  }
  if (IRdata == frontLightOFF) {
    LED.ToggleFrontLights(false);
  }
}

// Function for the vehicle to follow the line
void FollowLine() {
  if (sensors.DetectLine(reflectancePin1, "BLACK") && !sensors.DetectLine(reflectancePin2, "BLACK")) {
    // GO LEFT
    Serial.println("GO LEFT");
    motorControl.Drive(-250, 250);
  }
  if (sensors.DetectLine(reflectancePin2, "BLACK") && !sensors.DetectLine(reflectancePin1, "BLACK")) {
    // GO RIGHT
    Serial.println("GO RIGHT");
    motorControl.Drive(250, -250);
  }
  if(!sensors.DetectLine(reflectancePin1, "BLACK") && !sensors.DetectLine(reflectancePin2, "BLACK")) {
    // GO FORWARD
    Serial.println("GO FORWARD");
    motorControl.Drive(250, 250);
  }
  if(sensors.DetectLine(reflectancePin1, "BLACK") && sensors.DetectLine(reflectancePin2, "BLACK")) {
    // STOP
    Serial.println("STOP");
    motorControl.Stop();
  }
}
