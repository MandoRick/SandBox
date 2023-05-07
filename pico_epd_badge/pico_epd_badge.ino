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

/*
1 3v
2 gnd
3 SDA (19)
4 SCL (18)
5 CS  (15)
6 DC  (12)
7 RESET (13)
8 BUSY  (14)

#define EPD_WIDTH       176
#define EPD_HEIGHT      264

*/

#include <SPI.h>
#include "ER-EPM027-1B.h"
#include "imagedata.h"

const uint8_t dipCount = 6;
uint8_t dipPins[dipCount]{ 2, 3, 6, 7, 8, 9 };
bool dipArray[dipCount]{ 0, 0, 0, 0, 0, 0 };
const uint16_t sleepArrayMultiplier[dipCount]{ 60, 300, 600, 900, 1800, 3600 };  //1 min, 5 min, 10 min, 15 min, 30 mins, 1 hour
uint32_t sleepArrayCounter[dipCount]{ 0, 0, 0, 0, 0, 0 };
uint32_t sleepTimer = 10000;

Epd epd;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < dipCount; i++) {
    pinMode(dipPins[i], INPUT);
  }
  epd.Init();
  epd.DisplayFrame(LOGO);
  sleep_ms(sleepTimer);
  //test pins
  uint8_t testPins[6]{ 20, 21, 22, 26, 27, 28 };
  for (int i = 0; i < 6; i++) {
    pinMode(testPins[i], OUTPUT);
    digitalWrite(testPins[i], HIGH);
  }
}

void loop() {
  sleepTimer = 0;
  for (int i = 0; i < dipCount; i++) {
    dipArray[i] = 0;
    sleepArrayCounter[i] = 0;
    dipArray[i] = digitalRead(dipPins[i]);
    if (dipArray[i] == 1) {
      sleepArrayCounter[i] = sleepArrayMultiplier[i] * 1000;
      sleepTimer = sleepTimer + sleepArrayCounter[i];
    }
  }
  uint32_t sleepCalc = sleepTimer / 1000;
  sleepCalc = sleepCalc / 60;
  Serial.println("sleeping for: " + (String)sleepCalc + " minutes");
  epd.DisplayFrame(ID1);
  sleep_ms(sleepTimer);
  epd.DisplayFrame(ID2);
  sleep_ms(sleepTimer);
}
