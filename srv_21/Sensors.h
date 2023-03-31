#ifndef Sensors_h
#define Sensors_h

// IR Control Vars
#define IR_SMALLD_NEC         //1st: define which protocol to use and then,
#include <IRsmallDecoder.h>   //2nd: include the library;
IRsmallDecoder irDecoder(17); //3rd: create one decoder object with the correct digital pin;
irSmallD_t irData;            // IR data variable

// Gyro+Accelerometer MPU6050 Sensor Vars
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
//TwoWire CustomI2C0(20, 21);
Adafruit_MPU6050 mpu;

// Pololu QTRA Reflectance Sensor Array vars
#include <QTRSensors.h>
QTRSensors qtr;

class Sensors {

  public:
     // VARS
     bool keyIsHeld = false;
     bool irDataAvailable = false;
  
    // SETUP FUNCTIONS

    // Function to set up the MPU6050 gyro-accelerometer sensor
    bool SetupMPU(int gRange, int degRange, int bandwidth) {
      if (!mpu.begin(0x68)) {
        return false;
      }

      // Set up the acceleration range
      if (gRange <= 2) {
        mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
      }
      if (gRange <= 4 && gRange > 2) {
        mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
      }
      if (gRange <= 8 && gRange > 4) {
        mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
      }
      if (gRange > 8) {
        mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
      }

      // Set up the degree range
      if (degRange <= 250) {
        mpu.setGyroRange(MPU6050_RANGE_250_DEG);
      }
      if (degRange <= 500 && degRange > 250) {
        mpu.setGyroRange(MPU6050_RANGE_500_DEG);
      }
      if (degRange <= 1000 && degRange > 500) {
        mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
      }
      if (degRange > 1000) {
        mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
      }

      // Set up the bandwidth
      if (bandwidth == 260) {
        mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);
      }
      if (bandwidth == 184) {
        mpu.setFilterBandwidth(MPU6050_BAND_184_HZ);
      }
      if (bandwidth == 94) {
        mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
      }
      if (bandwidth == 21) {
        mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
      }
      if (bandwidth == 10) {
        mpu.setFilterBandwidth(MPU6050_BAND_10_HZ);
      }
      if (bandwidth == 5) {
        mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
      }

      return true;
    }

    
    // DATA GATHERING FUNCTIONS

    // Function to read the distance from the Ultrasonic Sensor
    float ReadDistance(int trigPin, int echoPin) {
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      float distance = pulseIn(echoPin, HIGH) / 58.00; //Equivalent to (340m/s*1us)/2
      return distance;
    }

    // Function to read the light intensity from the Photoresistor
    float ReadLightIntensity(int lightSensorPin) {
      return analogRead(lightSensorPin);
    }

    // Function to get the data from the IR Remote Controller
    String GetRemoteData() {
      keyIsHeld = irData.keyHeld;
      irDataAvailable = irDecoder.dataAvailable(irData);
      return DecodeKeyValue(irData.cmd);
    }

    // Function to get the acceleration value based on the axis (values are measured in m/s^2)
    float GetMPUAcceleration(String axis) {
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      if (axis == "X" || axis == "x") {
        return a.acceleration.x;
      }
      if (axis == "Y" || axis == "Y") {
        return a.acceleration.y;
      }
      if (axis == "Z" || axis == "z") {
        return a.acceleration.z;
      }
    }

    // Function to get the rotation rate values based on the axis (values are measured in deg/s)
    float GetMPURotation(String axis) {
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      if (axis == "X" || axis == "x") {
        return g.gyro.x * 180 / PI;
      }
      if (axis == "Y" || axis == "Y") {
        return g.gyro.y * 180 / PI;
      }
      if (axis == "Z" || axis == "z") {
        return g.gyro.z * 180 / PI;
      }
    }

    // Function to get the angle of MPU6050 based on the axis (values are measured in degrees)
    float GetMPURotationAngle(String axis){
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);

      if (axis == "X" || axis == "x") { // ROLL
        return atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
      }
      if (axis == "Y" || axis == "Y") { // PITCH
        return atan2(-a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180 / PI;
      }
      if (axis == "Z" || axis == "z") { // YAW
        return atan2(a.acceleration.z, sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y)) * 180 / PI;
      }
    }

    // Function to get the temperature value (value is measured in °C)
    float GetMPUTemperature() {
      sensors_event_t a, g, temp;
      mpu.getEvent(&a, &g, &temp);
      return temp.temperature;
    }

    // Function to get the values from the IR obstacle sensor
    int ReadReflectanceData(int sensorPin) {
      return analogRead(sensorPin);
    }

    // Function to detect the line using sensor connected to 'sensorPin'; Line color is defined as a parameter
    bool DetectLine(int sensorPin, String lineColor){
      if(lineColor == "BLACK" && ReadReflectanceData(sensorPin) > 80){
        return true;
      }
      if(lineColor == "WHITE" && ReadReflectanceData(sensorPin) < 80){
        return true;
      }
      return false;
    }


  private:

    // Function to decode key value of the IR result
    String DecodeKeyValue(long result) {
      switch (result) {
        case 0x16:
          return "0";
        case 0x0C:
          return "1";
        case 0x18:
          return "2";
        case 0x5E:
          return "3";
        case 0x08:
          return "4";
        case 0x1C:
          return "5";
        case 0x5A:
          return "6";
        case 0x42:
          return "7";
        case 0x52:
          return "8";
        case 0x4A:
          return "9";
        case 0x09:
          return "+";
        case 0x15:
          return "-";
        case 0x7:
          return "EQ";
        case 0x0D:
          return "U/SD";
        case 0x19:
          return "CYCLE";
        case 0x44:
          return "PLAY/PAUSE";
        case 0x43:
          return "FORWARD";
        case 0x40:
          return "BACKWARD";
        case 0x45:
          return "POWER";
        case 0x47:
          return "MUTE";
        case 0x46:
          return "MODE";
        default :
          return "ERROR";
      }
    }

};

#endif
