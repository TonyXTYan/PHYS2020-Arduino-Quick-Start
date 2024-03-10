/*
  Template SD Card data logger for ANU PHYS2020 Project
  Tony.Yan @anu.edu.au
  March 2024
*/
// SD Card Module 
#include <SPI.h>
#include <SD.h>
File myFile;  // declear a file object for reading/writing


// MAX6675 Thermocouple
#include "max6675.h"
MAX6675 thermocouple(6, 5, 4); // SCK - pin 6, CS - pin 5, SO - pin 4.


// DS18B20 Temperature Sensor 
#include <OneWire.h>
#include <DallasTemperature.h>
OneWire oneWire(2);         // setup a oneWire with DS18B20 DQ connected to pin 2 
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library


// MPU6050 Accelerometer and Gyroscope
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
Adafruit_MPU6050 mpu;


void setup()
{
  // Open serial communications with a baud rate of 2000000
  // You need to select 2000000 speed in Serial Monitor
  Serial.begin(2000000);
  Serial.println(); // print some lines to indicate a new session
  Serial.println();


  // MPU6050 
  if (!mpu.begin()) {
    Serial.println("MPU6050 chip initialisation failed!");
    while (1) { delay(10); } // TODO: gracefully handle this? currently this line would hault program if no MPU6050 found.
  }
  Serial.println("MPU6050 Initialised.");
  // You might want to tweak the following settings, 
  // I'm using setting from manufactuer's sample code.
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); 
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);


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
  logToSerialAndSD("Tms"); // Log milliseconds since startup
  logToSerialAndSD(millis());

  // MPU6050 
  sensors_event_t a, g, temp; // Get new sensor events with the readings
  mpu.getEvent(&a, &g, &temp);
  logToSerialAndSD("Acc");
  logToSerialAndSD(a.acceleration.x);
  logToSerialAndSD(a.acceleration.y);
  logToSerialAndSD(a.acceleration.z);
  logToSerialAndSD("Rot");
  logToSerialAndSD(g.gyro.x);
  logToSerialAndSD(g.gyro.y);
  logToSerialAndSD(g.gyro.z);


  // Pressure Sensor at Analog pin A1 
  logToSerialAndSD("V1");
  logToSerialAndSD(analogRead(A1)*5.0/1024.0); // Logging voltage derived from analogRead, you need to then convert this to pressure depending your gauge 

  // MAX6675
  logToSerialAndSD("TC");  
  logToSerialAndSD(thermocouple.readCelsius()); 

  // DS18B20
  tempSensor.requestTemperatures(); // send the command to get temperatures 
  logToSerialAndSD("TS");
  logToSerialAndSD(tempSensor.getTempCByIndex(0));


  // finishing loop
  logToSerialAndSD('\n'); // linebreak 
  myFile.flush(); // so that you can hot unplug SD card without losing data.
  delay(1); // delay 1ms, basically logging as fast as possible (about 8Hz), make delay longer if you want to less logs. 
}


// A helper function to log data to both Serial and SD card
template <typename T> // Allow logToSerialAndSD to accept any type
void logToSerialAndSD(T msg) {
  Serial.print(msg);
  if (msg != '\n') { Serial.print(','); } // add a comma for CSV format, unless it's a newline
  if (myFile) {
    myFile.print(msg);
    if (msg != '\n') { myFile.print(','); }// add a comma for CSV format, unless it's a newline
  } else {
    // Serial.println("Error logging to SD Card"); // commented out for gracefully running logger without SD card i.e. will only show SD card error in setup()
  }
}




