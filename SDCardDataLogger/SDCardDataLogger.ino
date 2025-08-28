/*
  Template SD Card data logger for ANU PHYS2020 Project
  Tony.Yan @anu.edu.au
  Aug 2025
*/
// SD Card Module 
#include <SPI.h>
#include <SD.h>
File myFile;  // declear a file object for reading/writing

// // MAX31855 Thermocouple 
// #include <SPI.h>
// #include "Adafruit_MAX31855.h"
// Adafruit_MAX31855 thermocouple2(5, 4, 3); // SCK - pin 5, CS - pin 4, DO - pin 3.

// // DS18B20 Temperature Sensor 
// #include <OneWire.h>
// #include <DallasTemperature.h>
// OneWire oneWire(2);         // setup a oneWire with DS18B20 DQ connected to pin 2 
// DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library

// // MPU6050 Accelerometer and Gyroscope
// #include <Adafruit_MPU6050.h>
// #include <Adafruit_Sensor.h>
// #include <Wire.h>
// Adafruit_MPU6050 mpu;

// ADXL345 Accelerometer
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);



void setup()
{
  // Open serial communications with a baud rate of 2000000
  // You need to select 2000000 speed in Serial Monitor
  Serial.begin(2000000);
  Serial.println(); // print some lines to indicate a new session
  Serial.println();


  // // MPU6050 
  // if (!mpu.begin()) {
  //   Serial.println("MPU6050 chip initialisation failed!");
  //   while (1) { delay(10); } // TODO: gracefully handle this? currently this line would hault program if no MPU6050 found.
  // }
  // Serial.println("MPU6050 Initialised.");
  // // You might want to tweak the following settings, 
  // // I'm using setting from manufactuer's sample code.
  // mpu.setAccelerometerRange(MPU6050_RANGE_8_G); 
  // mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  // mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);


  // ADXL345
  if(!accel.begin()) { 
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  accel.setRange(ADXL345_RANGE_16_G);


  // SD Card Module
  Serial.print("Initialising SD card...");
  if (!SD.begin(10)) { // SD Card module connected to pin 10 
    Serial.println("FAILED!");
    return; // Continue to loop() without SD card module 
  }
  Serial.println("done.");

  // Create a new log file for this session
  int index = 0;
  String fileName = "";
  do { // Find the next available log filename. 
    fileName = "log" + String(index) + ".csv";
    index++;
  } while (SD.exists(fileName)); // Check if file exists, if so, increment and try again
  Serial.print("Logging to: ");
  Serial.println(fileName);
  myFile = SD.open(fileName, FILE_WRITE); // Create and open the new log file 
  if (!myFile) {
    Serial.println("Could not create log file.");
  }
}


void loop()
{
  logToSerialAndSD("Tms", millis()); // Log milliseconds since startup

  // // MPU6050 
  // sensors_event_t a, g, temp; // Get new sensor events with the readings
  // mpu.getEvent(&a, &g, &temp);
  // logToSerialAndSD("Acc_X", a.acceleration.x);
  // logToSerialAndSD("Acc_Y", a.acceleration.y);
  // logToSerialAndSD("Acc_Z", a.acceleration.z);
  // logToSerialAndSD("Rot_X", g.gyro.x);
  // logToSerialAndSD("Rot_Y", g.gyro.y);
  // logToSerialAndSD("Rot_Z", g.gyro.z);

  // ADXL345
  sensors_event_t event; 
  accel.getEvent(&event);
  logToSerialAndSD("Acc_X", event.acceleration.x);
  logToSerialAndSD("Acc_Y", event.acceleration.y);
  logToSerialAndSD("Acc_Z", event.acceleration.z);

  // Pressure Sensor at Analog pin A1 
  logToSerialAndSD("V1", analogRead(A1)*5.0/1024.0); // Logging voltage derived from analogRead, you need to then convert this to pressure depending your gauge 

  // // MAX6675
  // logToSerialAndSD("TC", thermocouple.readCelsius()); 

  // // MAX31855 
  // logToSerialAndSD("TC2", thermocouple2.readCelsius());

  // // DS18B20
  // tempSensor.requestTemperatures(); // send the command to get temperatures 
  // logToSerialAndSD("TS", tempSensor.getTempCByIndex(0));


  // finishing loop
  logToSerialAndSD(); // linebreak 
  myFile.flush(); // so that you can hot unplug SD card without losing data.
  delay(1); // delay 1ms, basically logging as fast as possible (about 200Hz), increase delay if oversampling.
}


// Helper functions to log data to both Serial and SD card

// Zero input: new line
void logToSerialAndSD() {
  Serial.println();
  if (myFile) {
    myFile.println();
  } else {
    // Serial.println("Error logging to SD Card"); // commented out for gracefully running logger without SD card i.e. will only show SD card error in setup()
  }
}

// One input: text + comma
template <typename T>
void logToSerialAndSD(T msg) {
  Serial.print(msg);
  Serial.print(',');
  if (myFile) {
    myFile.print(msg);
    myFile.print(',');
  } else {
    // Serial.println("Error logging to SD Card"); // commented out for gracefully running logger without SD card i.e. will only show SD card error in setup()
  }
}

// Two inputs: text: value + comma
template <typename T1, typename T2>
void logToSerialAndSD(T1 label, T2 value) {
  Serial.print(label);
  Serial.print(":");
  Serial.print(value);
  Serial.print(',');
  if (myFile) {
    myFile.print(label);
    myFile.print(": ");
    myFile.print(value);
    myFile.print(',');
  } else {
    // Serial.println("Error logging to SD Card"); // commented out for gracefully running logger without SD card i.e. will only show SD card error in setup()
  }
}




