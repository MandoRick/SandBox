/*-data-type------size---------description-----
  boolean        (8 bit)   -  [true/false]
  byte           (8 bit)   -  [0-255] unsigned number
  char           (8 bit)   -  [-128 to 127] signed number
  unsigned char  (8 bit)   -  [-128 to 127] signed number
  word           (16 bit)  -  [0-65535] unsigned number
  unsigned int   (16 bit)  -  [0-65535] unsigned number
  int            (16 bit)  -  [-32768 to 32767] signed number
  unsigned long  (32 bit)  -  [0-4,294,967,295] unsigned number usually for millis
  long           (32 bit)  -  [-2,147,483,648 to 2,147,483,647] signed number
  float          (32 bit)  -  [-3.4028235E38 to 3.4028235E38] signed number
  uint8_t        (8 bit)   -  [0-255] unsigned number
  int8_t         (8 bit)   -  [-127 - 127] signed number
  uint16_t       (16 bit)  -  [0-65,535] unsigned number
  int16_t        (16 bit)  -  [-32,768 - 32,767] signed number
  uint32_t       (32 bit)  -  [0-4,294,967,295] unsigned number
  int32_t        (32 bit)  -  [-2,147,483,648 - 2,147,483,647] signed number
  uint64_t       (64 bit)  -  [0-18,446,744,073,709,551,615] unsigned number
  int64_t        (64 bit)  -  [−9,223,372,036,854,775,808 - 9,223,372,036,854,775,807] signed number
  --------------------------------------------
  camelCase                -  anything that changes
  snake_case               -  variable's that are exclusive in a function
  Snake_Case               -  CLASS/struct exclusave varables/functions
  iNVERTEDcAMELcASE        -  outside code that is being accessed [database]
  SNake_CAse               -  duplicate varables inside the case function [frequently used in library names]
  ALL_CAPS                 -  const varable names or defines
  -------------by-jediRick--------------------
*/

/*
  Formulae:
  r = m / t
  k = d × r × 0.001885

  Where:
  k = Kilometer Per Hour(km/hr)
  d = Wheel Diameter(cm)
  r = Revolution Per Minute(RPM)
  m = Millis 60000 per minute
  t = Time taken for rotation
*/

#define wheelDiam 28
#define sensorPin1 32
#define sensorPin2 35
#define tempSensor 34
#define pi 3.14159

#define debug 1     // 1 ON 0 OFF
#if debug == 1
#define debugln(x) Serial.println(x)
#define debug(x) Serial.print(x)
#else
#define debugln(x)
#define debug(x)
#endif

#include <SPI.h>
#include <TFT_eSPI.h>
#include "Free_Fonts.h"
TFT_eSPI tft = TFT_eSPI();

bool sensorState1 = true;
bool sensorState2 = true;
bool flipFlop = false;
bool wheelFlop = false;
bool udpateDisplay = true;

uint8_t kilosPerHour = 0;
unsigned long revsPerMin = 0;
float finalSpeedKph = 0;
uint32_t rotationCount;
float distanceTraveled;

unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned long rotationTime;
int16_t currentTemp;

//------- duration timer stuff --------
const unsigned long onTime1 = 100;
const unsigned long offTime1 = 400;
unsigned long previousMillis1 = 0;
unsigned long currentMillis1;
unsigned long interval1 = onTime1;
boolean triggerState1 = true;
//------- end duration timer stuff --------

void setup() {
  delay(2000);
  Serial.begin(115200);
  pinMode(sensorPin1, INPUT_PULLUP);
  pinMode(sensorPin2, INPUT_PULLUP);
  setupDisplay();
  delay(1000);
  currentMillis = millis();
  previousMillis = millis();
}

void loop() {
  calculateSpeed();
  drawDisplay();
  drawDuration();
}

void setupDisplay() {
  tft.init();
  tft.setRotation(1);
  tft.setFreeFont(FMB12);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setCursor(20, 45);
  tft.println("KPH");
  tft.setCursor(20, 85);
  tft.println("RPM");
  tft.setCursor(20, 125);
  tft.println("TEMP");
  tft.setCursor(20, 165);
  tft.println("DIST");
  tft.drawLine(0, 185, 320, 185, TFT_SKYBLUE);
  tft.setCursor(20, 220);
  tft.println("DUR");
}

void calculateSpeed() {
  if (digitalRead(sensorPin1) == 0) {
    //debugln("Sensor 1 LOW");
    sensorState1 = false;
  }
  if (digitalRead(sensorPin2) == 0) {
    //debugln("Sensor 2 LOW");
    sensorState2 = false;
  }
  if (sensorState1 == false && sensorState2 == false) {
    sensorState1 = true;
    sensorState2 = true;
    flipFlop = !flipFlop;
    if (flipFlop == true) {
      checkTemp();
      currentMillis = millis();
      rotationTime = currentMillis - previousMillis;
      revsPerMin = 60000 / rotationTime;
      previousMillis = currentMillis;
      rotationCount += 1;
      finalSpeedKph = wheelDiam * revsPerMin * 0.001885;
      distanceTraveled = ((wheelDiam * pi) * rotationCount) * 0.00001;
      udpateDisplay = true;
      debugln("Sensor triggered times: " + (String)rotationCount);
      debugln("Rotation time  millis: " + (String)rotationTime);
      debugln("Rotation RPM: " + (String)revsPerMin);
      debugln("Speed KPH: " + (String)finalSpeedKph);
      debugln("Distance: " + (String)distanceTraveled);
    }
  }
}

void checkTemp() {
  float tempPinVoltage = analogRead(tempSensor);
  //debugln("Temp Sensor Value " + (String)tempPinVoltage);
  tempPinVoltage = tempPinVoltage * 0.00122100;
  currentTemp = (tempPinVoltage - 0.5) * 100.0;
  debugln("temp C: " + (String)currentTemp);
}

void drawDisplay() {
  if (udpateDisplay == true) {
    tft.setFreeFont(FSB24);
    tft.fillRect(95, 0, 210, 183, TFT_BLACK);
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(100, 50);
    tft.println(finalSpeedKph);
    tft.setFreeFont(FSB18);
    tft.setCursor(100, 90);
    tft.println(revsPerMin);
    tft.setCursor(100, 130);
    tft.print(currentTemp);
    tft.println(" 'C");
    tft.setCursor(100, 170);
    tft.print(distanceTraveled);
    tft.println(" km");
    drawWheel();
    udpateDisplay = false;
  }
}

void drawWheel() {
  tft.fillCircle(290, 215 , 20, TFT_BLACK);
  if (wheelFlop == true) {
    tft.drawLine(280, 205, 300, 225, TFT_YELLOW);
    tft.drawLine(300, 205, 280, 225, TFT_YELLOW);
    wheelFlop = false;
  } else {
    tft.drawLine(290, 200, 290, 230, TFT_YELLOW);
    tft.drawLine(275, 215, 305, 215, TFT_YELLOW);
    wheelFlop = true;
  }
  tft.drawCircle(290, 215 , 15, TFT_ORANGE);
}

void drawDuration() {
  currentMillis1 = millis();
  long timeDuration = currentMillis1 / 1000;
  if ((unsigned long)(currentMillis1 - previousMillis1) >= interval1) {
    if (triggerState1) {
      interval1 = offTime1;
      tft.fillRect(95, 200, 170, 240, TFT_BLACK);
      tft.setTextColor(TFT_YELLOW);
      tft.setCursor(100, 225);
      tft.println(timeDuration);
    } else {
      interval1 = onTime1;
      //off time
    }
    triggerState1 = !(triggerState1);
    previousMillis1 = currentMillis1;
  }
}
