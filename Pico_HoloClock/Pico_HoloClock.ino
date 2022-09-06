/*-data-type------size---------description----------------------
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
  --------------------------------------------------------------
  camelCase                -  anything that changes
  snake_case               -  variable's that are exclusive in a function
  Snake_Case               -  CLASS/struct exclusave varables/functions
  iNVERTEDcAMELcASE        -  outside code that is being accessed [database]
  SNake_CAse               -  duplicate varables inside the case function [frequently used in library names]
  ALL_CAPS                 -  const varable names or defines
  ------------- by jediRick & RefreshMyMind --------------------
*/

#include <EEPROM.h>
#include <SPI.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include "Adafruit_ST7735.h" // Hardware-specific library for ST7735
#include "Adafruit_ST7789.h" // Hardware-specific library for ST7789

#define BUTTON_PIN_1 (2)
#define BUTTON_PIN_2 (3)
#define SECS_PER_MIN  (60UL)
#define NUM_SECS(_currentTime_) (_currentTime_ % SECS_PER_MIN)

Adafruit_ST7789 tft = Adafruit_ST7789(9, 8, 11, 10, 12);

int eepromAddress = 100;
int16_t textColor;
long currDay;
uint8_t currHour;
uint8_t currMin;
uint8_t currSec;
uint8_t currY = 100;
uint8_t prevY;
int8_t yIncrement = 1;
boolean flipFlop;
bool minLock = false;
bool prevbuttonHrs = false;
bool prevbuttonMin = false;

void setup() {
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  EEPROM.begin(512);
  //EEPROM.put(eepromAddress, 1);  //put something in memory to begin with
  currDay = EEPROM.get(eepromAddress, currDay);
  EEPROM.commit();
  EEPROM.end();
  tft.init(240, 320);
  tft.setRotation(1);
  tft.setTextSize(5);
  tft.fillScreen(ST77XX_BLACK);
}

void loop() {
  updateTime();
  checkButtons();
  displayTime();
}

void updateTime() {
  currSec = (millis() / 1000) % 60;
  if ((currSec == 0) && (minLock == false)) {
    currMin++;
    minLock = true;
  }
  else if (currSec != 0) {
    minLock = false;
  }
  if (currMin > 59) {
    currMin = 0;
    currHour++;
  }
  if (currHour > 23) {
    currHour = 0;
    currDay++;
    EEPROM.begin(512);
    EEPROM.put(eepromAddress, currDay);
    EEPROM.commit();
    EEPROM.end();
  }
}

void checkButtons() {
  if (digitalRead(BUTTON_PIN_1) == LOW && prevbuttonMin == true) {
    currMin++;
  }
  prevbuttonMin = digitalRead(BUTTON_PIN_1);
  if (digitalRead(BUTTON_PIN_2) == LOW && prevbuttonHrs == true) {
    currHour++;
  }
  prevbuttonHrs = digitalRead(BUTTON_PIN_2);
}

void displayTime() {  
  prevY = currY;
  currY = currY + yIncrement;
  if (currY <= 80 || currY >= 120) {
    yIncrement = -yIncrement;
  }
  if (currHour < 10) {
    tft.fillRect(10, prevY, 60, 40, ST77XX_BLACK);
    tft.setCursor(40, currY);
    tft.setTextColor(random(-32768, 32767));
    tft.print(currHour);
    tft.setCursor(10, currY);
    tft.print(0);
  } else {
    tft.fillRect(10, prevY, 60, 40, ST77XX_BLACK);
    tft.setCursor(10, currY);
    tft.setTextColor(random(-32768, 32767));
    tft.print(currHour);
  }
  if (currMin < 10) {
    tft.fillRect(140, prevY, 60, 40, ST77XX_BLACK);
    tft.setCursor(170, currY);
    tft.setTextColor(random(-32768, 32767));
    tft.print(currMin);
    tft.setCursor(140, currY);
    tft.print(0);
  } else {
    tft.fillRect(140, prevY, 60, 40, ST77XX_BLACK);
    tft.setCursor(140, currY);
    tft.setTextColor(random(-32768, 32767));
    tft.print(currMin);
  }
  if (currSec < 10) {
    tft.fillRect(250, prevY, 60, 40, ST77XX_BLACK);
    tft.setCursor(280, currY);
    tft.setTextColor(random(-32768, 32767));
    tft.print(currSec);
    tft.setCursor(250, currY);
    tft.print(0);
  } else {
    tft.fillRect(250, prevY, 60, 40, ST77XX_BLACK);
    tft.setCursor(250, currY);
    tft.setTextColor(random(-32768, 32767));
    tft.print(currSec);
  }
  if (flipFlop) {
    tft.fillRect(90, prevY, 30, 40, ST77XX_BLACK);
    tft.fillRect(210, prevY, 30, 40, ST77XX_BLACK);
    tft.setCursor(90, currY);
    tft.setTextColor(random(-32768, 32767));
    tft.print(":");
    tft.setCursor(210, currY);
    tft.setTextColor(random(-32768, 32767));
    tft.print(":");
    flipFlop = false;
  } else {
    tft.fillRect(90, prevY, 30, 40, ST77XX_BLACK);
    tft.fillRect(210, prevY, 30, 40, ST77XX_BLACK);
    flipFlop = true;
  }
  tft.setTextSize(2);
  tft.fillRect(20, prevY + 50, 80, 20, ST77XX_BLACK);
  tft.setCursor(20, currY + 50);
  tft.setTextColor(random(-32768, 32767));
  tft.print("millis:");
  tft.fillRect(120, prevY + 50, 120, 20, ST77XX_BLACK);
  tft.setCursor(120, currY + 50);
  tft.setTextColor(random(-32768, 32767));
  tft.print(millis());
  tft.fillRect(20, prevY + 70, 80, 20, ST77XX_BLACK);
  tft.setCursor(20, currY + 70);
  tft.setTextColor(random(-32768, 32767));
  tft.print("days:");
  tft.fillRect(120, prevY + 70, 120, 20, ST77XX_BLACK);
  tft.setCursor(120, currY + 70);
  tft.setTextColor(random(-32768, 32767));
  tft.print(currDay);
  tft.setTextSize(5);
  int lineColor = random(-32768, 32767);
  if (flipFlop) {
    for (int i = 120; i <= 220; i++) {
      tft.drawPixel(i + 20, 220, lineColor);
      tft.drawPixel(i + 20, 221, lineColor);
      tft.drawPixel(i, 220, ST77XX_BLACK);
      tft.drawPixel(i, 221, ST77XX_BLACK);
    }
  } else {
    for (int i = 220; i >= 120; i--) {
      tft.drawPixel(i - 20, 220, lineColor);
      tft.drawPixel(i - 20, 221, lineColor);
      tft.drawPixel(i, 220, ST77XX_BLACK);
      tft.drawPixel(i, 221, ST77XX_BLACK);
    }
  }
}
