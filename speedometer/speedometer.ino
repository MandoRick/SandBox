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
#include <ESP32Time.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "Free_Fonts.h"
TFT_eSPI tft = TFT_eSPI();
ESP32Time rtc;

#define screenOrientation 1

#define debugger 1     // 1 ON 0 OFF
#if debugger == 1
#define debugln(x) Serial.println(x)
#define debug(x) Serial.print(x)
#else
#define debugln(x)
#define debug(x)
#endif

#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24L)
#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)

#define wheelDiam 28
#define sensorPin1 32
#define sensorPin2 35
#define tempSensor 0
#define pi 3.14159


#define buttonPin1 2
#define buttonPin2 34
#define buttonPin3 25
#define buttonPin4 15
#define buttonPin5 33

int rtcCurrentHour = 0;
int rtcCurrentMinute = 0;
int rtcCurrentDay = 5;
int rtcCurrentMonth = 1;

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
const unsigned long onTime1 = 10;
const unsigned long offTime1 = 990;
unsigned long previousMillis1 = 0;
unsigned long currentMillis1;
unsigned long interval1 = onTime1;
boolean triggerState1 = true;
//------- end duration timer stuff --------

//------- second timer stuff --------
const unsigned long onTime2 = 10;
const unsigned long offTime2 = 90;
unsigned long previousMillis2 = 0;
unsigned long currentMillis2;
unsigned long interval2 = onTime2;
boolean triggerState2 = true;
//------- end second timer stuff --------

void setup() {
  delay(2000);
  Serial.begin(115200);
  rtc.setTime(00, rtcCurrentMinute, rtcCurrentHour, rtcCurrentDay, rtcCurrentMonth, 2022);
  pinMode(sensorPin1, INPUT_PULLUP);
  pinMode(sensorPin2, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(buttonPin5, INPUT_PULLUP);
  setupDisplay();
  delay(2000);
  currentMillis = millis();
  previousMillis = millis();
}

void(* resetFunc) (void) = 0;//declare reset function at address 0

void loop() {
  calculateSpeed();
  drawDisplay();
  drawDuration();
  wheelAnimationTiming();
}

void setupDisplay() {
  tft.init();
  //tft.setRotation(3);
  tft.setRotation(screenOrientation);
  tft.setFreeFont(FMB12);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_RED);
  tft.setCursor(20, 30);
  tft.println("KPH");
  tft.setCursor(20, 65);
  tft.println("RPM");
  tft.setCursor(20, 100);
  tft.println("TEMP");
  tft.setCursor(20, 135);
  tft.println("DIST");
  tft.drawLine(0, 160, 320, 160, TFT_SKYBLUE);
  tft.setCursor(20, 210);
  tft.println("TIME");
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
      debugln("Rotation time millis: " + (String)rotationTime);
      debugln("Rotation RPM: " + (String)revsPerMin);
      debugln("Speed KPH: " + (String)finalSpeedKph);
      debugln("Distance: " + (String)distanceTraveled);
    }
  }
}



void checkTemp() {
  float tempPinVoltage = analogRead(tempSensor);
  tempPinVoltage = tempPinVoltage * 0.00122100;
  currentTemp = (tempPinVoltage - 0.5) * 100.0;
  debugln("temp C: " + (String)currentTemp);
}

void drawDisplay() {
  //tft.init();
  if (udpateDisplay == true) {
    tft.setFreeFont(FSB24);
    tft.setTextColor(TFT_GREEN);
    tft.fillRect(95, 0, 170, 40, TFT_BLACK);
    tft.setCursor(100, 35);
    tft.println(finalSpeedKph);
    tft.setFreeFont(FSB18);
    tft.fillRect(95, 40, 170, 35, TFT_BLACK);
    tft.setCursor(100, 70);
    tft.println(revsPerMin);
    tft.fillRect(95, 75, 170, 35, TFT_BLACK);
    tft.setCursor(100, 105);
    tft.print(currentTemp);
    tft.println(" 'C");
    tft.fillRect(95, 110, 170, 35, TFT_BLACK);
    tft.setCursor(100, 140);
    tft.print(distanceTraveled);
    tft.println(" km");
    udpateDisplay = false;
  }
}

void drawDuration() {
  currentMillis1 = millis();
  if ((unsigned long)(currentMillis1 - previousMillis1) >= interval1) {
    if (triggerState1) {
      interval1 = offTime1;
      //off time
    } else {
      interval1 = onTime1;
      //off time
      tft.setFreeFont(FMB12);
      tft.setTextColor(TFT_YELLOW);
      tft.fillRect(95, 175, 170, 20, TFT_BLACK);
      tft.setCursor(128, 190);
      tft.println(":");
      tft.setCursor(170, 190);
      tft.println(":");
      tft.setCursor(100, 190);
      stopWatch(millis() / 1000);
      tft.fillRect(95, 195, 170, 20, TFT_BLACK);
      tft.setTextColor(TFT_PINK);
      tft.setCursor(100, 210);
      tft.println(rtc.getTime());
      tft.fillRect(95, 215, 170, 20, TFT_BLACK);
      tft.setCursor(100, 230);
      tft.println(rtc.getTime("%D"));
      struct tm timeinfo = rtc.getTimeStruct();
    }
    triggerState1 = !(triggerState1);
    previousMillis1 = currentMillis1;
  }
}

void wheelAnimationTiming() {
  currentMillis2 = millis();
  if ((unsigned long)(currentMillis2 - previousMillis2) >= interval2) {
    if (triggerState2) {
      interval2 = offTime2;
      //off time
    } else {
      interval2 = onTime2;
      checkButtons();
      drawWheel();
    }
    triggerState2 = !(triggerState2);
    previousMillis2 = currentMillis2;
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

void stopWatch(long val) {
  int hours = numberOfHours(val);
  int minutes = numberOfMinutes(val);
  int seconds = numberOfSeconds(val);
  printDigits(hours);
  printDigits(minutes);
  printDigits(seconds);
  tft.println();
  debugln();
}

void printDigits(byte digits) {
  // utility function for digital clock display: prints colon and leading 0
  debug(":");
  if (digits < 10)
    tft.print('0');
  debug('0');
  tft.print(digits, DEC);
  debug(digits);
  tft.print(" ");
}

void checkButtons() {
  bool buttonState1 = digitalRead(buttonPin1);
  bool buttonState2 = digitalRead(buttonPin2);
  bool buttonState3 = digitalRead(buttonPin3);
  bool buttonState4 = digitalRead(buttonPin4);
  bool buttonState5 = digitalRead(buttonPin5);
  if (buttonState1 == HIGH) {
    debugln("Button 1 Pressed");
    rtcCurrentHour = rtcCurrentHour + 1;    
    rtc.setTime(00, rtcCurrentMinute, rtcCurrentHour, rtcCurrentDay, rtcCurrentMonth, 2022);
  }
  if (buttonState2 == HIGH) {
    debugln("Button 2 Pressed");
    rtcCurrentMinute = rtcCurrentMinute + 1;
    rtc.setTime(00, rtcCurrentMinute, rtcCurrentHour, rtcCurrentDay, rtcCurrentMonth, 2022); 
  }
  if (buttonState3 == HIGH) {    
    debugln("Button 3 Pressed");
        rtcCurrentMonth = rtcCurrentMonth + 1;
    rtc.setTime(00, rtcCurrentMinute, rtcCurrentHour, rtcCurrentDay, rtcCurrentMonth, 2022);
  }
  if (buttonState4 == HIGH) {
    debugln("Button 4 Pressed");
        rtcCurrentDay = rtcCurrentDay + 1;
    rtc.setTime(00, rtcCurrentMinute, rtcCurrentHour, rtcCurrentDay, rtcCurrentMonth, 2022);
  }
  if (buttonState5 == HIGH) {
    debugln("Button 5 Pressed");
    resetFunc(); //call reset 
  }
}
