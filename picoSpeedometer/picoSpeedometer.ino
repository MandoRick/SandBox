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

#define debug 1     // 1 ON 0 OFF
#if debug == 1
#define debugln(x) Serial.println(x)
#define debug(x) Serial.print(x)
#else
#define debugln(x)
#define debug(x)
#endif
#define ledPin  LED_BUILTIN

#include <Wire.h>
#include <DS3231.h>
#include <MPU6050_light.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "notes.h"

#define wheelDiam 28
#define pi 3.14159
#define hallSensorPin1 4
#define hallSensorPin2 5
#define pin_buzzer 16
#define red_light_pin 18
#define green_light_pin 19
#define blue_light_pin 20
#define led LED_BUILTIN
#define buttonPin1 15
#define buttonPin2 17
#define buttonPin3 2
#define buttonPin4 3
Adafruit_ST7789 tft = Adafruit_ST7789(9, 8, 11, 10, 12);
RTClib myRTC;
DS3231 Clock;
int selectedScreen = 1;
byte alarmHour = 0;
byte alarmMinute = 0;
bool alarmActive = false;
bool wipeScreen = false;
bool fireAlarm = false;
uint16_t animationPos = 0;
uint16_t prevAnimationPos;
int animationMoveAmount = 5;
//--------------------------
uint32_t counter = 0;
int randNumber1;
int randNumber2;
int color = 0xF800;
int brightness = 0;    // how bright the LED is
int fadeAmount = 1;    // how many points to fade the LED by
//--------------------------
//------- rtc stuff --------
byte currentHour;
byte currentMinute;
byte currentSecond;
byte currentDay;
byte currentMonth;
word currentYear;
float currentTemp;

byte oldHour;
byte oldMinute;
byte oldSecond;
byte oldDay;
byte oldMonth;
word oldYear;
float oldTemp;


unsigned long currentMillis;

//--------------------------
//------- button stuff --------
bool buttonState1;
bool buttonState2;
bool buttonState3;
bool buttonState4;
bool button1pressed;
bool button2pressed;
bool button3pressed;
bool button4pressed;
bool button1released;
bool button2released;
bool button3released;
bool button4released;
//--------------------------
//------- timer stuff --------
const unsigned long onTime1 = 10;
const unsigned long offTime1 = 100;
unsigned long previousMillis1 = 0;
unsigned long interval1 = onTime1;
boolean triggerState1 = true;
const unsigned long onTime2 = 10;
const unsigned long offTime2 = 100;
unsigned long previousMillis2 = 0;
unsigned long interval2 = onTime2;
boolean triggerState2 = true;
//--------------------------

//------- version stuff --------
const String p_project = "default";
const uint8_t version_hi = 0;
const uint8_t version_lo = 1;
void versionPrint(void) {
  debug("RicksWorx: ");
  debugln(p_project);
  debug("Version: ");
  debug(version_hi);
  debug(".");
  debugln(version_lo);
}
//------- end version stuff --------

bool sensorState1 = true;
bool sensorState2 = true;
bool flipFlop = false;
bool wheelFlop = false;
bool udpateDisplay = true;


uint8_t kilosPerHour = 0;
unsigned long revsPerMin = 0;
float finalSpeedKph = 0;
float oldSpeed;
uint32_t rotationCount;
float distanceTraveled = 0;
float oldDistance;
unsigned long rotationTime;
//unsigned long currentMillis;
unsigned long previousMillis = 0;

//------- tone generation --------
void playNote(int frequency, int duration, bool hold = false, bool measure = true) {
  (void) measure;
  if (hold) {
    // For a note that's held play it a little longer than the specified duration
    // so it blends into the next tone (but there's still a small delay to
    // hear the next note).
    tone(pin_buzzer, frequency, duration + duration / 32);
  } else {
    // For a note that isn't held just play it for the specified duration.
    tone(pin_buzzer, frequency, duration);
  }
  delay(duration + duration / 16);
}
//--------------------------

//------- setup one --------
void setup() {
  delay(2000);
  versionPrint();
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();
  randomSeed(analogRead(26));
  tft.init(240, 320);
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);
}
//--------------------------

//------- setup two --------
void setup1() {
  delay(2000);
  Serial.begin(115200);
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
  pinMode(pin_buzzer, OUTPUT);
  pinMode(hallSensorPin1, INPUT_PULLUP);
  pinMode(hallSensorPin2, INPUT_PULLUP);
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
  pinMode(led, OUTPUT);
}
//--------------------------

//------- loop one --------
void loop() {
  drawScreen();
}
//--------------------------

//------- loop two --------
void loop1() {
  calculateSpeed();
  checkTimer2();
  checkButtons();
  checkAlarm();
  //RGB_color(random(20), random(20), random(20));
  // set the brightness
  analogWrite(led, brightness);
  //RGB_color(random(80), random(80), random(80));
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(5);
  if (currentMinute == 0 && currentSecond == 0) {
    RGB_color(random(80), random(80), random(80));
    playNote(NOTE_B6, SIXTEENTH, false);
    RGB_color(0, 0, 0);
  }
}
//--------------------------



void calculateSpeed() {
  if (digitalRead(hallSensorPin1) == 0) {
    //debugln("Sensor 1 LOW");
    sensorState1 = false;
  }
  if (digitalRead(hallSensorPin2) == 0) {
    //debugln("Sensor 2 LOW");
    sensorState2 = false;
  }
  if (sensorState1 == false && sensorState2 == false) {
    sensorState1 = true;
    sensorState2 = true;
    flipFlop = !flipFlop;
    if (flipFlop == true) {
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



//------- timer1 one --------
void checkTimer1() {
  currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis1) >= interval1) {
    if (triggerState1) {
      interval1 = offTime1;
    } else {
      drawTime();
    }
    triggerState1 = !(triggerState1);
    previousMillis1 = currentMillis;
  }
}
//--------------------------

//------- draw screen --------
void drawScreen() {
  if (selectedScreen == 1) {
    checkTimer1();
    if (wipeScreen) {
      tft.fillRect(0, 0, 320, 240, ST77XX_BLACK);
      wipeScreen = false;
    }
    color += random(-10000, 10000);
    /*prevAnimationPos = animationPos;
      //tft.fillRect(animationPos, 200 , 5, 5, ST77XX_BLACK);
      animationPos = animationPos + animationMoveAmount;
      tft.fillRect(animationPos, 225 , 20, 10, color);
      if (animationPos <= 0 || animationPos >= 300) {
      animationMoveAmount = -animationMoveAmount;
      }
      tft.drawCircle(random(50, tft.width() - 50), random(150, tft.height() - 50), random(25), color);
      tft.fillCircle(random(50, tft.width() - 50), random(150, tft.height() - 50), random(25), ST77XX_BLACK);
      tft.drawRect(random(50, tft.width() - 50), random(150, tft.height() - 50) , random(25), random(25), color);
      tft.fillRect(random(50, tft.width() - 50), random(150, tft.height() - 50) , random(25), random(25), ST77XX_BLACK);
    */
  }
  if (selectedScreen == 2) {
    if (wipeScreen) {
      tft.fillRect(0, 0, 320, 240, ST77XX_BLACK);
      wipeScreen = false;
    }
    checkTimer1();
    color += random(-10000, 10000);
    tft.setTextColor(color);
    tft.setTextWrap(true);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println("+ Hour");
    tft.setCursor(260, 0);
    tft.println("Min +");
    tft.setCursor(130, 0);
    tft.println("Alarm");
    tft.setCursor(0, 220);
    tft.println("+ Set");
    tft.fillRect(10, 140, 300, 70, ST77XX_BLACK);
    tft.setTextSize(4);
    tft.setCursor(120, 150);
    tft.println((String)alarmHour + ":" + (String)alarmMinute);
    tft.setTextSize(2);
    tft.setCursor(120, 190);
    tft.println("Alarm: " + (String)alarmActive);
    delay(250);
  }
  if (selectedScreen == 3) {
    if (wipeScreen) {
      tft.fillRect(0, 0, 320, 240, ST77XX_BLACK);
      wipeScreen = false;
    }
    checkTimer1();
    color += random(-10000, 10000);
    tft.setTextColor(color);
    tft.setTextWrap(true);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println("+ Hour");
    tft.setCursor(260, 0);
    tft.println("Min +");
    tft.setCursor(130, 0);
    tft.println("Time");
    delay(250);
  }
  if (selectedScreen == 4) {
    if (wipeScreen) {
      tft.fillRect(0, 0, 320, 240, ST77XX_BLACK);
      wipeScreen = false;
    }
    checkTimer1();
    color += random(-10000, 10000);
    tft.setTextColor(color);
    tft.setTextWrap(true);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.println("+ Month");
    tft.setCursor(260, 0);
    tft.println("Day +");
    tft.setCursor(130, 0);
    tft.println("Date");
    tft.setCursor(0, 220);
    tft.println("+ Year");
    delay(250);
  }
}
//--------------------------

//------- draw time --------
void drawTime() {
  interval1 = onTime1;
  DateTime now = myRTC.now();
  currentHour = now.hour();
  currentMinute = now.minute();
  currentSecond = now.second();
  currentDay = now.day();
  currentMonth = now.month();
  currentYear = now.year();
  currentTemp = analogReadTemp();

  tft.setTextWrap(false);
  tft.setTextSize(1);
  tft.setCursor(0, 30);
  tft.println("Time");
  tft.setCursor(0, 62);
  tft.println("Date");
  tft.setCursor(0, 95);
  tft.println("Temp");
  tft.setCursor(0, 127);
  tft.println("Dist");
  tft.setCursor(0, 160);
  tft.println("KpH");
  tft.setTextSize(3);
  if (oldHour != currentHour) {
    tft.setCursor(70, 30);
    //tft.drawRect(70, 20 , 65, 35, ST77XX_RED);
    tft.fillRect(70, 30 , 40, 25, ST77XX_BLACK);
    tft.drawRect(70, 30 , 40, 25, ST77XX_RED);
    tft.println((String)currentHour);
  } else {
    tft.setCursor(70, 30);
    tft.drawRect(70, 30 , 40, 25, ST77XX_RED);
    tft.println((String)currentHour);
  }
  if (oldMinute != currentMinute) {
    tft.setCursor(105, 30);
    //tft.drawRect(135, 20 , 90, 35, ST77XX_RED);
    tft.fillRect(110, 30 , 50, 25, ST77XX_BLACK);
    tft.drawRect(110, 30 , 50, 25, ST77XX_RED);
    tft.println(":" + (String)currentMinute);
  } else {
    tft.setCursor(105, 30);
    tft.drawRect(110, 30 , 50, 25, ST77XX_RED);
    tft.println(":" + (String)currentMinute);
  }
  if (oldSecond != currentSecond) {
    tft.setCursor(155, 30);
    //tft.drawRect(225, 20 , 90, 35, ST77XX_RED);
    tft.fillRect(160, 30 , 50, 25, ST77XX_BLACK);
    tft.drawRect(160, 30 , 50, 25, ST77XX_RED);
    tft.println(":" + (String)currentSecond);
  } else {
    tft.setCursor(155, 30);
    tft.drawRect(160, 30 , 50, 25, ST77XX_RED);
    tft.println(":" + (String)currentSecond);
  }
  if (oldDay != currentDay) {
    tft.setTextSize(2);
    tft.setCursor(70, 65);
    //tft.drawRect(70, 65 , 40, 25, ST77XX_RED);
    tft.fillRect(70, 65 , 40, 15, ST77XX_BLACK);
    tft.drawRect(70, 65 , 40, 15, ST77XX_RED);
    tft.println((String)currentDay);
  } else {
    tft.setTextSize(2);
    tft.setCursor(70, 65);
    tft.drawRect(70, 65 , 40, 15, ST77XX_RED);
    tft.println((String)currentDay);
  }
  if (oldMonth != currentMonth) {
    tft.setTextSize(2);
    tft.setCursor(110, 65);
    //tft.drawRect(110, 65 , 60, 25, ST77XX_RED);
    tft.fillRect(110, 65 , 40, 15, ST77XX_BLACK);
    tft.drawRect(110, 65 , 40, 15, ST77XX_RED);
    tft.println("/" + (String)currentMonth);
  } else {
    tft.setTextSize(2);
    tft.setCursor(110, 65);
    tft.drawRect(110, 65 , 40, 15, ST77XX_RED);
    tft.println("/" + (String)currentMonth);
  }
  if (oldYear != currentYear) {
    tft.setTextSize(2);
    tft.setCursor(150, 65);
    //tft.drawRect(170, 65 , 90, 25, ST77XX_RED);
    tft.fillRect(150, 65 , 60, 15, ST77XX_BLACK);
    tft.drawRect(150, 65 , 60, 15, ST77XX_RED);
    tft.println("/" + (String)currentYear);
  } else {
    tft.setTextSize(2);
    tft.setCursor(150, 65);
    tft.drawRect(150, 65 , 60, 15, ST77XX_RED);
    tft.println("/" + (String)currentYear);
  }
  if (oldTemp != currentTemp) {
    tft.setTextSize(2);
    tft.setCursor(70, 95);
    //tft.drawRect(70, 95 , 100, 20, ST77XX_RED);
    tft.fillRect(70, 95 , 70, 15, ST77XX_BLACK);
    tft.drawRect(70, 95 , 70, 15, ST77XX_RED);
    tft.println((String)currentTemp);
  } else {
    tft.setTextSize(2);
    tft.setCursor(70, 95);
    tft.drawRect(70, 95 , 70, 15, ST77XX_RED);
    tft.println((String)currentTemp);

  }

  if (oldDistance != distanceTraveled) {
    tft.setTextSize(4);
    tft.setCursor(70, 125);
    //tft.drawRect(70, 95 , 100, 20, ST77XX_RED);
    tft.fillRect(70, 125 , 150, 30, ST77XX_BLACK);
    tft.drawRect(70, 125 , 150, 30, ST77XX_RED);
    tft.println((String)distanceTraveled);
  } else {
    tft.setTextSize(4);
    tft.setCursor(70, 125);
    tft.drawRect(70, 125 , 150, 30, ST77XX_RED);
    tft.println((String)distanceTraveled);

  }
  int currentSpeed = finalSpeedKph;
  if (oldSpeed != currentSpeed) {
    tft.setTextSize(10);
    tft.setCursor(70, 160);
    //tft.drawRect(70, 95 , 100, 20, ST77XX_RED);
    tft.fillRect(70, 160 , 110, 70, ST77XX_BLACK);
    tft.drawRect(70, 160 , 110, 70, ST77XX_RED);
    tft.println((String)currentSpeed);
  } else {
    tft.setTextSize(10);
    tft.setCursor(70, 160);
    tft.drawRect(70, 160 , 110, 70, ST77XX_RED);
    tft.println((String)currentSpeed);

  }
  oldDistance = distanceTraveled;
  oldSpeed = currentSpeed;
  oldHour = currentHour;
  oldMinute = currentMinute;
  oldSecond = currentSecond;
  oldTemp = currentTemp;
  oldDay = currentDay;
  oldMonth = currentMonth;
  oldYear = currentYear;
}
//--------------------------

//------- timer two --------
void checkTimer2() {
  if ((unsigned long)(currentMillis - previousMillis2) >= interval2) {
    if (triggerState2) {
      interval2 = offTime2;
    } else {
      interval2 = onTime2;
    }
    triggerState2 = !(triggerState2);
    previousMillis2 = currentMillis;
  }
}
//--------------------------

//------- RGB control --------
void RGB_color(int red_light_value, int green_light_value, int blue_light_value) {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
//--------------------------

//------- buttons check --------
void checkButtons() {
  buttonState1 = digitalRead(buttonPin1);     //read buttons
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
  if (BOOTSEL) {
    color += random(-10000, 10000);
    tft.setTextColor(color);
  }
  if (buttonState1 == LOW) {                  //check if button has been pressed
    button1pressed = true;
  }
  if (buttonState2 == LOW) {
    button2pressed = true;
  }
  if (buttonState3 == LOW) {
    button3pressed = true;
  }
  if (buttonState4 == LOW) {
    button4pressed = true;
  }
  if (buttonState1 == HIGH) {                 //check if button has been released
    button1released = true;
  }
  if (buttonState2 == HIGH) {
    button2released = true;
  }
  if (buttonState3 == HIGH) {
    button3released = true;
  }
  if (buttonState4 == HIGH) {
    button4released = true;
  }
  if (button1pressed == true && button1released == true) {    //if button has been pressed AND released, do a thing
    button1pressed = false;
    button1released = false;
    Serial.println("Button A Pressed and action performed");
    playNote(NOTE_B6, SIXTEENTH, false);
    if (selectedScreen == 2) {
      alarmHour = alarmHour + 1;
      if (alarmHour == 24) {
        alarmHour = 0;
      }
    }
    if (selectedScreen == 3) {
      currentHour = currentHour + 1;
      if (currentHour == 24) {
        currentHour = 0;
      }
      Clock.setHour(currentHour);
    }
    if (selectedScreen == 4) {
      currentDay = currentDay + 1;
      if (currentDay == 32) {
        currentDay = 1;
      }
      Clock.setDate(currentDay);
    }
    delay(250);  //action delay
  }
  if (button2pressed == true && button2released == true) {
    button2pressed = false;
    button2released = false;
    Serial.println("Button B Pressed and action performed");
    playNote(NOTE_B6, SIXTEENTH, false);
    if (selectedScreen == 2) {
      alarmMinute = alarmMinute + 1;
      if (alarmMinute == 60) {
        alarmMinute = 0;
      }
    }
    if (selectedScreen == 3) {
      currentMinute = currentMinute + 1;
      if (currentMinute == 60) {
        currentMinute = 0;
      }
      Clock.setMinute(currentMinute);
    }
    if (selectedScreen == 4) {
      currentMonth = currentMonth + 1;
      if (currentMonth == 13) {
        currentMonth = 1;
      }
      Clock.setMonth(currentMonth);
    }
    delay(250);  //action delay
  }
  if (button3pressed == true && button3released == true) {
    button3pressed = false;
    button3released = false;
    Serial.println("Button C Pressed and action performed");
    if (selectedScreen == 2) {
    }
    alarmActive = !alarmActive;
    fireAlarm = false;
    playNote(NOTE_B6, SIXTEENTH, false);
    if (selectedScreen == 4) {
      currentYear = currentYear + 1;
      if (currentYear >= 2040) {
        currentYear = 21;
      }
      Clock.setYear(currentYear);
    }
    delay(250);  //action delay
  }
  if (button4pressed == true && button4released == true) {
    button4pressed = false;
    button4released = false;
    Serial.println("Button D Pressed and action performed");
    playNote(NOTE_B6, SIXTEENTH, false);
    wipeScreen = true;
    selectedScreen = selectedScreen + 1;
    if (selectedScreen == 5) {
      selectedScreen = 1;
    }
    Serial.println("Active Screen = " + (String)selectedScreen);
    delay(250);  //action delay
  }
}
//--------------------------

//------- alarm check --------
void checkAlarm() {
  if (alarmActive && alarmHour == currentHour && alarmMinute == currentMinute) {
    fireAlarm = true;
  }
  soundAlarm();
}
//--------------------------

void soundAlarm() {
  if (fireAlarm && alarmActive) {
    RGB_color(random(80), random(80), random(80));
    playNote(NOTE_B6, SIXTEENTH, false);
    RGB_color(random(80), random(80), random(80));
    playNote(NOTE_B2, SIXTEENTH, false);
    RGB_color(random(80), random(80), random(80));
    playNote(NOTE_B2, SIXTEENTH, false);
    RGB_color(0, 0, 0);
  }
}
