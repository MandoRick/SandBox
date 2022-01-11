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

  #define color0 ST77XX_BLACK
  #define color1 ST77XX_WHITE
  #define color2 ST77XX_RED
  #define color3 ST77XX_GREEN
  #define color4 ST77XX_BLUE
  #define color5 ST77XX_CYAN
  #define color6 ST77XX_MAGENTA
  #define color7 ST77XX_YELLOW
  #define color8 ST77XX_ORANGE

*/

#define mydebug 1     // 1 ON 0 OFF
#if mydebug == 1
#define debugln(x) Serial.println(x)
#define debug(x) Serial.print(x)
#else
#define debugln(x)
#define debug(x)
#endif

#include <Wire.h>
#include <DS3231.h>
#include <MPU6050_light.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "notes.h"

#define wirePin1 0
#define wirePin2 1

#define hallSensorPin1 4
#define hallSensorPin2 5

#define built_in_led  LED_BUILTIN
#define red_led_pin 18
#define green_led_pin 19
#define blue_led_pin 20

#define pin_buzzer 16

#define wheelDiam 28  //in cm
#define pi 3.14159

#define buttonPin1 (15)
#define buttonPin2 (17)
#define buttonPin3 (2)
#define buttonPin4 (3)

#define color0 ST77XX_BLACK
#define color1 ST77XX_WHITE
#define color2 ST77XX_RED
#define color3 ST77XX_GREEN
#define color4 ST77XX_BLUE
#define color5 ST77XX_CYAN
#define color6 ST77XX_MAGENTA
#define color7 ST77XX_YELLOW
#define color8 ST77XX_ORANGE

Adafruit_ST7789 tft = Adafruit_ST7789(9, 8, 11, 10, 12);
RTClib myRTC;
DS3231 Clock;

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
const unsigned long onTime1 = 1;
const unsigned long offTime1 = 1;
unsigned long previousMillis1 = 0;
unsigned long interval1 = onTime1;
boolean triggerState1 = true;
const unsigned long onTime2 = 1;
const unsigned long offTime2 = 1;
unsigned long previousMillis2 = 0;
unsigned long currentMillis;
unsigned long interval2 = onTime2;
boolean triggerState2 = true;
//--------------------------

bool sensorState1 = true;
bool sensorState2 = true;
bool flipFlop = false;
uint8_t kilosPerHour = 0;
unsigned long revsPerMin = 0;
float finalSpeedKph = 0;
uint32_t rotationCount;
float distanceTraveled = 0;
unsigned long rotationTime;
//  current millis in timer stuff
unsigned long previousMillis = 0;
bool udpateDisplay = true;
uint8_t currentSpeed;
float currentDistance;
uint32_t currentRevs;
uint32_t currentSpeedRender;
float currentDistanceRender;
uint32_t currentRevsRender;
float oldDistance;
uint8_t oldSpeed;
uint32_t oldRevs;

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
//--------------------------

int8_t displayColorCount1 = 1;
int8_t displayColorCount2 = 2;
uint16_t displayColor1 = color1;
uint16_t displayColor2 = color2;

uint8_t modeCount = 1;
bool setupHour = false;
bool setupMinute = false;
bool setupSecond = false;
bool setupDay = false;
bool setupMonth = false;
bool setupYear = false;
bool editMode = false;
bool completedDrawing = true;
bool completedErasing = true;
String editState = "Hour";

void setup() {
  delay(2000);
  setupSerial();
  versionPrint();
  setupLEDs();
  setupDisplay();
  setupAudio();
  setupWire();
  setupButtons();
}

void setup1() {
  delay(2000);
  setupSpeedSenors();
}

void loop() {
  RGB_color(random(80), random(80), random(80));
  checkTimer1();  //get speed value and time during off time, draw display during on time
}

void loop1() {
  readSpeedSensors();
  checkTimer2();  //set speed value during on time, check buttons during off time
}

void setupDisplay() {
  tft.init(240, 320);
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(false);
  tft.setTextColor(displayColor2);
  tft.setTextSize(1);
  tft.setCursor(0, 30);
  tft.println("Time");
  tft.setCursor(0, 62);
  tft.println("Date");
  tft.setCursor(0, 95);
  tft.println("Temp");
  tft.setCursor(0, 127);
  tft.println("Dist");
  tft.setCursor(0, 170);
  tft.println("KpH");
  tft.setCursor(0, 260);
  tft.println("W-RPM");
}

void redrawMargin() {
  tft.setTextColor(displayColor2);
  tft.setTextSize(1);
  tft.setCursor(0, 30);
  tft.println("Time");
  tft.setCursor(0, 62);
  tft.println("Date");
  tft.setCursor(0, 95);
  tft.println("Temp");
  tft.setCursor(0, 127);
  tft.println("Dist");
  tft.setCursor(0, 170);
  tft.println("KpH");
  tft.setCursor(0, 260);
  tft.println("W-RPM");
}

void setupAudio() {
  pinMode(pin_buzzer, OUTPUT);
}

void setupWire() {
  Wire.setSDA(wirePin1);
  Wire.setSCL(wirePin2);
  Wire.begin();
}

void setupButtons() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  pinMode(buttonPin4, INPUT_PULLUP);
}

void setupSerial() {
  Serial.begin(115200);
  delay(250);
  debugln("Serial started");
  delay(250);
}

void setupLEDs() {
  pinMode(built_in_led, OUTPUT);
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);
  pinMode(blue_led_pin, OUTPUT);
}

void setupSpeedSenors() {
  pinMode(hallSensorPin1, INPUT_PULLUP);
  pinMode(hallSensorPin2, INPUT_PULLUP);
}

void checkTimer1() {
  currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis1) >= interval1) {
    if (triggerState1) {
      interval1 = offTime1;
      //debugln("Timer 1 off");
      getSpeedValue();
      updateTime();
      drawDisplay();
    } else {
      interval1 = onTime1;
      //debugln("Timer 1 on");
      checkButtons();
    }
    triggerState1 = !(triggerState1);
    previousMillis1 = currentMillis;
  }
}

void getSpeedValue() {
  currentSpeedRender = currentSpeed;
  currentDistanceRender = currentDistance;
  currentRevsRender = currentRevs;
  //debugln("Rotation RPM: " + (String)currentRevsRender);
  //debugln("Speed KPH: " + (String)currentSpeedRender);
  //debugln("Distance: " + (String)currentDistanceRender);
}

void updateTime() {
  interval1 = onTime1;
  DateTime now = myRTC.now();
  currentHour = now.hour();
  currentMinute = now.minute();
  currentSecond = now.second();
  currentDay = now.day();
  currentMonth = now.month();
  currentYear = now.year();
  currentTemp = analogReadTemp();
}

void drawDisplay() {
  tft.setTextColor(displayColor1);
  if (editMode == true) {
    drawSetupLabel();
  } else {
    eraseSetupLabel();
  }

  if (oldHour != currentHour) {
    tft.fillRect(70, 30 , 40, 25, ST77XX_BLACK);
    tft.setTextSize(3);
    tft.setCursor(70, 30);
    tft.println((String)currentHour);
  } else {
    tft.setTextSize(3);
    tft.setCursor(70, 30);
    tft.println((String)currentHour);
  }

  if (oldMinute != currentMinute) {
    tft.fillRect(110, 30 , 50, 25, ST77XX_BLACK);
    tft.setTextSize(3);
    tft.setCursor(105, 30);
    tft.println(":" + (String)currentMinute);
  } else {
    tft.setTextSize(3);
    tft.setCursor(105, 30);
    tft.println(":" + (String)currentMinute);
  }

  if (oldSecond != currentSecond) {
    tft.fillRect(160, 30 , 50, 25, ST77XX_BLACK);
    tft.setTextSize(3);
    tft.setCursor(155, 30);
    tft.println(":" + (String)currentSecond);
  } else {
    tft.setTextSize(3);
    tft.setCursor(155, 30);
    tft.println(":" + (String)currentSecond);
  }

  if (oldDay != currentDay) {
    tft.fillRect(70, 65 , 40, 15, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setCursor(70, 65);
    tft.println((String)currentDay);
  } else {
    tft.setTextSize(2);
    tft.setCursor(70, 65);
    tft.println((String)currentDay);
  }

  if (oldMonth != currentMonth) {
    tft.fillRect(110, 65 , 40, 15, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setCursor(110, 65);
    tft.println("/" + (String)currentMonth);
  } else {
    tft.setTextSize(2);
    tft.setCursor(110, 65);
    tft.println("/" + (String)currentMonth);
  }

  if (oldYear != currentYear) {
    tft.fillRect(150, 65 , 60, 15, ST77XX_BLACK);
    tft.setTextSize(2);
    tft.setCursor(150, 65);
    tft.println("/" + (String)currentYear);
  } else {
    tft.setTextSize(2);
    tft.setCursor(150, 65);
    tft.println("/" + (String)currentYear);
  }

  if (oldTemp != currentTemp) {
    tft.fillRect(70, 95 , 70, 15, ST77XX_BLACK);
    //tft.drawRect(70, 95 , 70, 15, ST77XX_RED);
    tft.setTextSize(2);
    tft.setCursor(70, 95);
    tft.println((String)currentTemp);
  } else {
    //tft.drawRect(70, 95 , 70, 15, ST77XX_RED);
    tft.setTextSize(2);
    tft.setCursor(70, 95);
    tft.println((String)currentTemp);
  }

  if (oldDistance != currentDistanceRender) {
    tft.fillRect(70, 125 , 150, 30, ST77XX_BLACK);
    //tft.drawRect(70, 125 , 150, 30, ST77XX_RED);
    tft.setTextSize(4);
    tft.setCursor(70, 125);
    tft.println((String)currentDistanceRender);
  } else {
    //tft.drawRect(70, 125 , 150, 30, ST77XX_RED);
    tft.setTextSize(4);
    tft.setCursor(70, 125);
    tft.println((String)currentDistanceRender);
  }

  if (oldSpeed != currentSpeedRender) {
    tft.fillRect(70, 170 , 110, 70, ST77XX_BLACK);
    //tft.drawRect(70, 170 , 110, 70, ST77XX_RED);
    tft.setTextSize(10);
    tft.setCursor(70, 170);
    tft.println((String)currentSpeedRender);
  } else {
    //tft.drawRect(70, 170 , 110, 70, ST77XX_RED);
    tft.setTextSize(10);
    tft.setCursor(70, 170);
    tft.println((String)currentSpeedRender);
  }

  if (oldRevs != currentRevs) {
    tft.fillRect(70, 260 , 150, 30, ST77XX_BLACK);
    //tft.drawRect(70, 260 , 150, 30, ST77XX_RED);
    tft.setTextSize(4);
    tft.setCursor(70, 260);
    tft.println((String)currentRevs);
  } else {
    //tft.drawRect(70, 260 , 150, 30, ST77XX_RED);
    tft.setTextSize(4);
    tft.setCursor(70, 260);
    tft.println((String)currentRevs);
  }

  oldDistance = currentDistanceRender;
  oldSpeed = currentSpeedRender;
  oldRevs = currentRevsRender;
  oldHour = currentHour;
  oldMinute = currentMinute;
  oldSecond = currentSecond;
  oldTemp = currentTemp;
  oldDay = currentDay;
  oldMonth = currentMonth;
  oldYear = currentYear;
}

void checkTimer2() {
  currentMillis = millis();
  if ((unsigned long)(currentMillis - previousMillis1) >= interval1) {
    if (triggerState1) {
      interval1 = offTime1;
    } else {
      interval1 = onTime1;
      //debugln("Timer 2 on");
      setSpeedValue();
    }
    triggerState1 = !(triggerState1);
    previousMillis1 = currentMillis;
  }
}

void readSpeedSensors() {
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
    }
  }
}

void setSpeedValue() {
  currentSpeed = finalSpeedKph;
  currentDistance = distanceTraveled;
  currentRevs = revsPerMin;
}

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

void checkButtons() {
  buttonState1 = digitalRead(buttonPin1);     //read buttons
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  buttonState4 = digitalRead(buttonPin4);
  if (buttonState1 == LOW) {                  //check if button 1 has been pressed
    button1pressed = true;
    button1released = false;
  } else if (buttonState1 == HIGH) {
    button1released = true;
  }
  if (button1pressed == true && button1released == true) {    //if button has been pressed AND released, do a thing
    button1pressed = false;
    button1released = false;
    debugln("Button 1 Pressed!");
    playNote(NOTE_B6, SIXTEENTH, false);
    modeCount++;
    if (modeCount >= 9) {
      modeCount = 1;
    }
    if (modeCount == 1) {
      editState = "Hour";
    } else if (modeCount == 2) {
      editState = "Minute";
    } else if (modeCount == 3) {
      editState = "Second";
    } else if (modeCount == 4) {
      editState = "Day";
    } else if (modeCount == 5) {
      editState = "Month";
    } else if (modeCount == 6) {
      editState = "Year";
    } else if (modeCount == 7) {
      editState = "Color 1";
    } else if (modeCount == 8) {
      editState = "Color 2";
    }
    if (editMode == true) {
      completedDrawing = !completedDrawing;
    }
    if (editMode == false) {
      completedErasing = !completedErasing;
    }
  }
  //---------
  if (buttonState2 == LOW) {                  //check if button 1 has been pressed
    button2pressed = true;
    button2released = false;
  } else if (buttonState2 == HIGH) {
    button2released = true;
  }
  if (button2pressed == true && button2released == true) {    //if button has been pressed AND released, do a thing
    button2pressed = false;
    button2released = false;
    debugln("Button 2 Pressed!");
    playNote(NOTE_B6, SIXTEENTH, false);
    if (modeCount == 1 && editMode == true) {
      currentHour = currentHour - 1;
      Clock.setHour(currentHour);
    }
    if (modeCount == 2 && editMode == true) {
      currentMinute = currentMinute - 1;
      Clock.setMinute(currentMinute);
    }
    if (modeCount == 3 && editMode == true) {
      currentSecond = currentSecond - 1;
      Clock.setMinute(currentMinute);
    }
    if (modeCount == 4 && editMode == true) {
      currentDay = currentDay - 1;
      if (currentDay == 0) {
        currentDay = 31;
      }
      Clock.setDate(currentDay);
    }
    if (modeCount == 5 && editMode == true) {
      currentMonth = currentMonth - 1;
      if (currentMonth == 0) {
        currentMonth = 12;
      }
      Clock.setMonth(currentMonth);
    }
    if (modeCount == 6 && editMode == true) {
      currentYear = currentYear - 1;
      if (currentYear == 21) {
        currentYear = 99;
      }
      Clock.setYear(currentYear);
    }
    if (modeCount == 7 && editMode == true) {
      displayColorCount1 = displayColorCount1 - 1;
      if (displayColorCount1 == 9) {
        displayColorCount1 = 1;
      } else if (displayColorCount1 == 0) {
        displayColorCount1 = 8;
      }
      if (displayColorCount1 == 1) {
        displayColor1 = color1;
      } else if (displayColorCount1 == 2) {
        displayColor1 = color2;
      } else if (displayColorCount1 == 3) {
        displayColor1 = color3;
      } else if (displayColorCount1 == 4) {
        displayColor1 = color4;
      } else if (displayColorCount1 == 5) {
        displayColor1 = color5;
      } else if (displayColorCount1 == 6) {
        displayColor1 = color6;
      } else if (displayColorCount1 == 7) {
        displayColor1 = color7;
      } else if (displayColorCount1 == 8) {
        displayColor1 = color8;
      }
      tft.setTextColor(displayColor1);
    }
    if (modeCount == 8 && editMode == true) {
      displayColorCount2 = displayColorCount2 - 1;
      if (displayColorCount2 == 9) {
        displayColorCount2 = 1;
      } else if (displayColorCount2 == 0) {
        displayColorCount2 = 8;
      }
      if (displayColorCount2 == 1) {
        displayColor2 = color1;
      } else if (displayColorCount2 == 2) {
        displayColor2 = color2;
      } else if (displayColorCount2 == 3) {
        displayColor2 = color3;
      } else if (displayColorCount2 == 4) {
        displayColor2 = color4;
      } else if (displayColorCount2 == 5) {
        displayColor2 = color5;
      } else if (displayColorCount2 == 6) {
        displayColor2 = color6;
      } else if (displayColorCount2 == 7) {
        displayColor2 = color7;
      } else if (displayColorCount2 == 8) {
        displayColor2 = color8;
      }
      tft.setTextColor(displayColor2);
      redrawMargin();
    }
  }










  //---------
  if (buttonState3 == LOW) {                  //check if button 1 has been pressed
    button3pressed = true;
    button3released = false;
  } else if (buttonState3 == HIGH) {
    button3released = true;
  }
  if (button3pressed == true && button3released == true) {    //if button has been pressed AND released, do a thing
    button3pressed = false;
    button3released = false;
    debugln("Button 3 Pressed!");
    playNote(NOTE_B6, SIXTEENTH, false);
    if (modeCount == 1 && editMode == true) {
      currentHour = currentHour + 1;
      if (currentHour == 25) {
        currentHour = 1;
      }
      Clock.setHour(currentHour);
    }
    if (modeCount == 2 && editMode == true) {
      currentMinute = currentMinute + 1;
      if (currentMinute == 60) {
        currentMinute = 0;
      }
      Clock.setMinute(currentMinute);
    }
    if (modeCount == 3 && editMode == true) {
      currentSecond = currentSecond + 1;
      if (currentMinute == 60) {
        currentMinute = 0;
      }
      Clock.setMinute(currentMinute);
    }
    if (modeCount == 4 && editMode == true) {
      currentDay = currentDay + 1;
      if (currentDay == 32) {
        currentDay = 1;
      }
      Clock.setDate(currentDay);
    }
    if (modeCount == 5 && editMode == true) {
      currentMonth = currentMonth + 1;
      if (currentMonth == 13) {
        currentMonth = 1;
      }
      Clock.setMonth(currentMonth);
    }
    if (modeCount == 6 && editMode == true) {
      currentYear = currentYear + 1;
      if (currentYear >= 2100) {
        currentYear = 2022;
      }
      Clock.setYear(currentYear);
    }
    if (modeCount == 7 && editMode == true) {
      displayColorCount1 = displayColorCount1 + 1;
      if (displayColorCount1 == 9) {
        displayColorCount1 = 1;
      } else if (displayColorCount1 == 0) {
        displayColorCount1 = 8;
      }
      if (displayColorCount1 == 1) {
        displayColor1 = color1;
      } else if (displayColorCount1 == 2) {
        displayColor1 = color2;
      } else if (displayColorCount1 == 3) {
        displayColor1 = color3;
      } else if (displayColorCount1 == 4) {
        displayColor1 = color4;
      } else if (displayColorCount1 == 5) {
        displayColor1 = color5;
      } else if (displayColorCount1 == 6) {
        displayColor1 = color6;
      } else if (displayColorCount1 == 7) {
        displayColor1 = color7;
      } else if (displayColorCount1 == 8) {
        displayColor1 = color8;
      }
      tft.setTextColor(displayColor1);
    }
    if (modeCount == 8 && editMode == true) {
      displayColorCount2 = displayColorCount2 + 1;
      if (displayColorCount2 == 9) {
        displayColorCount2 = 1;
      } else if (displayColorCount2 == 0) {
        displayColorCount2 = 8;
      }
      if (displayColorCount2 == 1) {
        displayColor2 = color1;
      } else if (displayColorCount2 == 2) {
        displayColor2 = color2;
      } else if (displayColorCount2 == 3) {
        displayColor2 = color3;
      } else if (displayColorCount2 == 4) {
        displayColor2 = color4;
      } else if (displayColorCount2 == 5) {
        displayColor2 = color5;
      } else if (displayColorCount2 == 6) {
        displayColor2 = color6;
      } else if (displayColorCount2 == 7) {
        displayColor2 = color7;
      } else if (displayColorCount2 == 8) {
        displayColor2 = color8;
      }
      tft.setTextColor(displayColor2);
      redrawMargin();
    }
  }
  //---------
  if (buttonState4 == LOW) {                  //check if button 1 has been pressed
    button4pressed = true;
    button4released = false;
  } else if (buttonState4 == HIGH) {
    button4released = true;
  }
  if (button4pressed == true && button4released == true) {    //if button has been pressed AND released, do a thing
    button4pressed = false;
    button4released = false;
    debugln("Button 4 Pressed!");
    playNote(NOTE_B6, SIXTEENTH, false);
    editMode = !editMode;
    if (editMode == true) {
      completedDrawing = !completedDrawing;
    }
    if (editMode == false) {
      completedErasing = !completedErasing;
    }
  }
  //---------
  if (BOOTSEL) {
    displayColor2 += random(-10000, 10000);
    tft.setTextColor(displayColor2);
    redrawMargin();
    displayColor1 += random(-10000, 10000);
    tft.setTextColor(displayColor1);
  }
  //---------
}

void drawSetupLabel() {
  if (completedDrawing == false) {
    tft.fillRect(10, 5 , 60, 10, ST77XX_BLACK);
    //tft.drawRect(10, 0 ,  60, 10, ST77XX_RED);
    tft.fillRect(100, 5 , 50, 10, ST77XX_BLACK);
    //tft.drawRect(100, 0 , 40, 10, ST77XX_RED);
    tft.setTextColor(displayColor2);
    tft.setTextSize(1);
    tft.setCursor(10, 5);
    tft.println("SetupMode: ");
    tft.setCursor(100, 5);
    tft.println(editState);
  }
  completedDrawing = true;
}

void eraseSetupLabel() {
  if (completedErasing == false) {
    tft.fillRect(10, 5 , 60, 10, ST77XX_BLACK);
    tft.fillRect(100, 5 , 50, 10, ST77XX_BLACK);
  }
  completedErasing = true;
}

//------- RGB control --------
void RGB_color(int red_led_value, int green_led_value, int blue_led_value) {
  analogWrite(red_led_pin, red_led_value);
  analogWrite(green_led_pin, green_led_value);
  analogWrite(blue_led_pin, blue_led_value);
}
//--------------------------
