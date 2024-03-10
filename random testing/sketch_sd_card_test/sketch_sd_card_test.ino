// SD Card
#include <SPI.h>
#include <SD.h>
File myFile;  // setup a file reader/writer instance

// MAX6675
#include "max6675.h"
MAX6675 thermocouple(6, 5, 4); // SCK - pin 6, CS - pin 5, SO - pin 4.


// DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>
//  Arduino pin 2 connected to DS18B20 sensor's DQ pin
OneWire oneWire(2);         // setup a oneWire instance
DallasTemperature tempSensor(&oneWire); // pass oneWire to DallasTemperature library


// MPU6050 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
Adafruit_MPU6050 mpu;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(2000000);
  while (!Serial) {
    delay(10); 
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.println();
  Serial.println();
  Serial.print("Initializing SD card...");

  if (!SD.begin(10)) { // SD Card module connected to pin 10 
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");


  // Create a new log file for this session
  String fileName = createNewLogFile();
  Serial.print("Logging to: ");
  Serial.println(fileName);

  // // open the file. note that only one file can be open at a time,
  // // so you have to close this one before opening another.
  // myFile = SD.open("test.txt", FILE_WRITE);

  // // if the file opened okay, write to it:
  // if (myFile) {
  //   Serial.print("Writing to test.txt...");
  //   myFile.println("testing 1, 2, 3.");
  //   // close the file:
  //   myFile.close();
  //   Serial.println("done.");
  // } else {
  //   // if the file didn't open, print an error:
  //   Serial.println("error opening test.txt");
  // }

  // // re-open the file for reading:
  // myFile = SD.open("test.txt");
  // if (myFile) {
  //   Serial.println("test.txt:");

  //   // read from the file until there's nothing else in it:
  //   while (myFile.available()) {
  //     Serial.write(myFile.read());
  //   }
  //   // close the file:
  //   myFile.close();
  // } else {
  //   // if the file didn't open, print an error:
  //   Serial.println("error opening test.txt");
  // }



  // Try to initialize! MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
  // You might want to tweak the following settings
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); 
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

}

void loop()
{
  // nothing happens after setup


  // MPU6050 
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");

  Serial.println("");



  Serial.println(millis());

  Serial.print("V=");
  Serial.println(analogRead(A1) * 5 / 1023.0); 

  Serial.print("C1 = "); 
  Serial.println(thermocouple.readCelsius());

  tempSensor.requestTemperatures();             // send the command to get temperatures
  // float tempCelsius = tempSensor.getTempCByIndex(0);  // read temperature in Celsius
  Serial.print("C2 = "); 
  Serial.println(tempSensor.getTempCByIndex(0));

  delay(1000);

}


void logToSerialAndSD(String msg) {
  Serial.println(msg);
  myFile = SD.open("testlog.csv", FILE_WRITE);
  if (myFile) {
    myFile.print(msg);
    myFile.close();
  } else {
    Serial.println("Error opening file");
  }

}



String createNewLogFile() {
  // Starting file index
  int index = 0;
  String fileName = "";

  do {
    // Generate new file name
    fileName = "log" + String(index) + ".txt";
    index++;
  } while (SD.exists(fileName)); // Check if file exists, if so, increment and try again

  // Create new file and immediately close it
  myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    myFile.close();
  } else {
    Serial.println("Could not create log file.");
  }

  return fileName; // Return the new file name
}



