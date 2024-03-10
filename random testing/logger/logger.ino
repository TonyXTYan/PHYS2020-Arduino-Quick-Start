////  OLED
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//#include "SSD1306Ascii.h" // fonts?
//#include "SSD1306AsciiAvrI2c.h"
//#include <Fonts/FreeMono9pt7b.h>


#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// OLED SDA to Analog A4 FIXED
// OLED SCK to Analog A5 FIXED

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//Adafruit_SSD1306 oled(128, 32, &Wire, -1);
//// END OLED


//// THERMOCOUPLE MAX31855
#include <SPI.h>
#include "Adafruit_MAX31855.h"

// Thermocouple 1
#define MAXDO   2
#define MAXCS_1 4
#define MAXCLK  3
Adafruit_MAX31855 thermocouple_1(MAXCLK, MAXCS_1, MAXDO);

// Thermocouple2
#define MAXCS_2   5
Adafruit_MAX31855 thermocouple_2(MAXCLK, MAXCS_2, MAXDO);

// Thermocouple3
#define MAXCS_3   6
Adafruit_MAX31855 thermocouple_3(MAXCLK, MAXCS_3, MAXDO);

// Thermocouple4
#define MAXCS_4   7
Adafruit_MAX31855 thermocouple_4(MAXCLK, MAXCS_4, MAXDO);

// Thermocouple5
#define MAXCS_5   8
Adafruit_MAX31855 thermocouple_5(MAXCLK, MAXCS_5, MAXDO);
//// END THERMOCOUPLE


//// Miscellaneous 
const int buttonPin = 13;   // Push button
const int ledPin =  13;     // the number of the LED pin
int buttonState = 0;        // Push button state
// RGB LED
const int pinRed   = 10;
const int pinGreen = 11;
const int pinBlue  = 12;


void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  
  Serial.begin(9600);
  while (!Serial) delay(10);
  Serial.println(F("Program Start"));
  delay(2000);  // waiting for stuff

  Serial.println(F("Setting up OLED"));
  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("ERROR: SSD1306 allocation failed"));
    while (true) delay(10);
  }

  delay(2000);         // wait for initializing
  oled.clearDisplay(); // clear display

  oled.setTextSize(1);          // text size
//  oled.setFont();
//  oled.setFont(&FreeMono9pt7b);
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 10);        // position to display
  oled.println(F("Setting up"));   // text to display
  oled.display();               // show on OLED
  

  Serial.println(F("Setting up Thermocouple MAX31855"));
  if (!thermocouple_1.begin()) {
    Serial.println(F("ERROR: while setting up thermocouple 1 MAX31855 "));
    while (true) delay(10);
  }
  if (!thermocouple_2.begin()) {
    Serial.println(F("ERROR: while setting up thermocouple 2 MAX31855 "));
    while (true) delay(10);
  }
  if (!thermocouple_3.begin()) {
    Serial.println(F("ERROR: while setting up thermocouple 3 MAX31855 "));
    while (true) delay(10);
  }
  if (!thermocouple_4.begin()) {
    Serial.println(F("ERROR: while setting up thermocouple 4 MAX31855 "));
    while (true) delay(10);
  }
  if (!thermocouple_5.begin()) {
    Serial.println(F("ERROR: while setting up thermocouple 5 MAX31855 "));
    while (true) delay(10);
  }

  
  Serial.println(F("Setup done :) "));

  Serial.println(F("Timestamp,Log,Button,Int1,Int2,Int3,Int4,Int5,Temp1,Temp2,Temp3,Temp4,Temp5,"));
}

int counter = 0;

double thermocouple_temp_1 = -1.0;
double thermocouple_temp_2 = -1.0;
double thermocouple_temp_3 = -1.0;
double thermocouple_temp_4 = -1.0;
double thermocouple_temp_5 = -1.0;

void loop() {
  digitalWrite(ledPin, LOW);
  color(0,200,0);
  Serial.print(F(","));
  Serial.print(counter);
  Serial.print(F(","));

  updateButtonStatus();
  updateThermocouples();
  color(0,200,200);
  updateOledStatus();
  counter++;
  Serial.print(F("\n"));
//  delay(10);
}


void updateOledStatus(){
  oled.clearDisplay();
  oled.setCursor(0, 0); 
  oled.print(F("Log "));
  oled.println(counter);
  oled.print(F("1: "));
  oled.println(thermocouple_temp_1);
  oled.print(F("2: "));
  oled.println(thermocouple_temp_2);
  oled.print(F("3: "));
  oled.println(thermocouple_temp_3);
  oled.print(F("4: "));
  oled.println(thermocouple_temp_4);
  oled.print(F("5: "));
  oled.println(thermocouple_temp_5);
  if (buttonState == LOW){
    oled.println(F("BUTTON"));
  }
  oled.display();
}

void updateButtonStatus() {
    // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
//    digitalWrite(ledPin, HIGH);
//    Serial.print(F("Button, UP, "));
    Serial.print(F("False, "));
  } else {
//    digitalWrite(ledPin, LOW);
    color(200,200,200);
//    Serial.print(F("Button, PRESSED, "));
    Serial.print(F("True, "));
  }
}


void updateThermocouples() {
  thermocouple_temp_1 = thermocouple_1.readCelsius();
  thermocouple_temp_2 = thermocouple_2.readCelsius();
  thermocouple_temp_3 = thermocouple_3.readCelsius();
  thermocouple_temp_4 = thermocouple_4.readCelsius();
  thermocouple_temp_5 = thermocouple_5.readCelsius();

//  Serial.print(F("1i, "));
  Serial.print(thermocouple_1.readInternal());
  Serial.print(F(","));
//  Serial.print(F("2i, "));
  Serial.print(thermocouple_2.readInternal());
  Serial.print(F(","));
//  Serial.print(F("3i, "));
  Serial.print(thermocouple_3.readInternal());
  Serial.print(F(","));
//  Serial.print(F("4i, "));
  Serial.print(thermocouple_4.readInternal());
  Serial.print(F(","));
//  Serial.print(F("5i, "));
  Serial.print(thermocouple_5.readInternal());
  Serial.print(F(","));

  if (isnan(thermocouple_temp_1)) {       // 1
//   Serial.print(F("1t, ERR, "));
   Serial.print(F("NaN,"));
  } else {
//   Serial.print(F("1t, "));
   Serial.print(thermocouple_temp_1);
   Serial.print(F(","));
  }
  
  if (isnan(thermocouple_temp_2)) {       // 2
//   Serial.print(F("2t, ERR, "));
   Serial.print(F("NaN,"));
  } else {
//   Serial.print(F("2t, "));
   Serial.print(thermocouple_temp_2);
   Serial.print(F(","));
  }

  if (isnan(thermocouple_temp_3)) {       // 3
//   Serial.print(F("3t, ERR, "));
   Serial.print(F("NaN,"));
  } else {
//   Serial.print(F("3t, "));
   Serial.print(thermocouple_temp_3);
   Serial.print(F(","));
  }

  if (isnan(thermocouple_temp_4)) {       // 4
//   Serial.print(F("4t, ERR, "));
   Serial.print(F("NaN,"));
  } else {
//   Serial.print(F("4t, "));
   Serial.print(thermocouple_temp_4);
   Serial.print(F(","));
  }

  if (isnan(thermocouple_temp_5)) {       // 5
//   Serial.print(F("5t, ERR, "));
   Serial.print(F("NaN, "));
  } else {
//   Serial.print(F("5t, "));
   Serial.print(thermocouple_temp_2);
   Serial.print(F(", "));
  }
  
}

void color(unsigned char red, unsigned char green, unsigned blue) {
  analogWrite(pinRed, red);
  analogWrite(pinBlue, blue);
  analogWrite(pinGreen, green);
}
