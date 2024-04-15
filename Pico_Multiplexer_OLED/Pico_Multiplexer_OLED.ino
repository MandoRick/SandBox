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

#include <Wire.h>
#include <U8g2lib.h>
#include <Adafruit_TSC2007.h>
#include <Keyboard.h>

#include "display01.h"
#include "display02.h"
#include "display03.h"
#include "display04.h"
#include "display05.h"
#include "display06.h"
#include "display07.h"
#include "display08.h"
#include "display09.h"
#include "display10.h"
#include "display11.h"
#include "display12.h"
#include "display13.h"
#include "display14.h"
#include "display15.h"
#include "display16.h"
#include "display17.h"
#include "display18.h"
#include "display19.h"
#include "display20.h"
#include "display21.h"
#include "display22.h"
#include "display23.h"
#include "display24.h"

#define DISP_QTY_PER_MUX 8
#define NUM_MULTIPLEXERS 3
#define DISP_QTY_TOTAL 24
#define I2C_SPEED 800000L

#define DISP_H 72
#define DISP_W 40

uint8_t muxAddressPins[NUM_MULTIPLEXERS] = { 10, 11, 12 };
uint8_t multiplexerAddresses[NUM_MULTIPLEXERS] = { 0x71, 0x72, 0x73 };

uint16_t touchCoordinates[4] = { 0, 0, 0, 0 };

bool displaySelected[DISP_QTY_TOTAL] = {};
bool oldDisplaySelected[DISP_QTY_TOTAL] = {};

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

Adafruit_TSC2007 touch;

void TCA9548A(uint8_t address, uint8_t bus) {
  Wire.beginTransmission(address);
  Wire.write(1 << bus);
  Wire.endTransmission();
}

void setup() {
  randomSeed(analogRead(26));
  setupMuxAddressPins();
  delay(1000);
  setupDisplays();
  delay(1000);
  touch.begin();
  delay(1000);
}

void loop() {
  readTouch();
  renderDisplays();
}

void setupMuxAddressPins() {
  for (int i = 0; i < NUM_MULTIPLEXERS; i++) {
    pinMode(muxAddressPins[i], OUTPUT);
    delay(10);
    digitalWrite(muxAddressPins[i], 1);
    delay(10);
  }
}

void setupDisplays() {
  Wire.begin();
  Wire.setClock(I2C_SPEED);
  for (int multiplexer = 0; multiplexer < NUM_MULTIPLEXERS; multiplexer++) {
    for (int display = 0; display < DISP_QTY_PER_MUX; display++) {
      TCA9548A(multiplexerAddresses[multiplexer], display);
      u8g2.begin();
      u8g2.setFont(u8g2_font_6x10_tf);
      u8g2.setFontRefHeightExtendedText();
      u8g2.setDrawColor(1);
      u8g2.setFontPosTop();
      u8g2.setFontDirection(0);
      u8g2.clearBuffer();
      u8g2.drawStr(10, 0, ("DISPLAY: " + String(display + multiplexer * DISP_QTY_PER_MUX)).c_str());
      u8g2.sendBuffer();
      delay(250);
    }
  }
  for (int display = 0; display < DISP_QTY_TOTAL; display++) {
    displaySelected[display] = false;
    oldDisplaySelected[display] = false;
  }
}

void renderDisplays() {
  for (int display = 0; display < DISP_QTY_TOTAL; display++) {
    displaySelected[display] = random(2);
  }
  int displayIndex = 0;
  for (int multiplexer = 0; multiplexer < NUM_MULTIPLEXERS; multiplexer++) {
    for (int display = 0; display < DISP_QTY_PER_MUX; display++) {
      if (displaySelected[displayIndex] != oldDisplaySelected[displayIndex]) {
        TCA9548A(multiplexerAddresses[multiplexer], display);
        u8g2.clearBuffer();
        if (displaySelected[displayIndex]) {
          for (int i = 0; i < DISP_W; i++) {
            for (int j = 0; j < DISP_H; j++) {
              if (getOnPixel(displayIndex, i, j)) {
                u8g2.drawPixel(j, i);
              }
            }
          }
        } else {
          for (int i = 0; i < DISP_W; i++) {
            for (int j = 0; j < DISP_H; j++) {
              if (getOffPixel(displayIndex, i, j)) {
                u8g2.drawPixel(j, i);
              }
            }
          }
        }
        u8g2.sendBuffer();
        //delay(25);
        //Keyboard.print("Display ");
        //Keyboard.print(displayIndex);
        //Keyboard.print(": ");
        //Keyboard.print(displaySelected[displayIndex]);
        //Keyboard.write(KEY_RETURN);
        displayIndex++;
      }
    }
  }
  for (int display = 0; display < DISP_QTY_TOTAL; display++) {
    oldDisplaySelected[display] = displaySelected[display];
  }
}

void readTouch() {
  touch.read_touch(&touchCoordinates[0], &touchCoordinates[1], &touchCoordinates[2], &touchCoordinates[3]);
}

bool getOnPixel(int displayIndex, int x, int y) {
  switch (displayIndex) {
    case 0:
      return Pico_Deck_Display01_ON[x][y];
    case 1:
      return Pico_Deck_Display02_ON[x][y];
    case 2:
      return Pico_Deck_Display03_ON[x][y];
    case 3:
      return Pico_Deck_Display04_ON[x][y];
    case 4:
      return Pico_Deck_Display05_ON[x][y];
    case 5:
      return Pico_Deck_Display06_ON[x][y];
    case 6:
      return Pico_Deck_Display07_ON[x][y];
    case 7:
      return Pico_Deck_Display08_ON[x][y];
    case 8:
      return Pico_Deck_Display09_ON[x][y];
    case 9:
      return Pico_Deck_Display10_ON[x][y];
    case 10:
      return Pico_Deck_Display11_ON[x][y];
    case 11:
      return Pico_Deck_Display12_ON[x][y];
    case 12:
      return Pico_Deck_Display13_ON[x][y];
    case 13:
      return Pico_Deck_Display14_ON[x][y];
    case 14:
      return Pico_Deck_Display15_ON[x][y];
    case 15:
      return Pico_Deck_Display16_ON[x][y];
    case 16:
      return Pico_Deck_Display17_ON[x][y];
    case 17:
      return Pico_Deck_Display18_ON[x][y];
    case 18:
      return Pico_Deck_Display19_ON[x][y];
    case 19:
      return Pico_Deck_Display20_ON[x][y];
    case 20:
      return Pico_Deck_Display21_ON[x][y];
    case 21:
      return Pico_Deck_Display22_ON[x][y];
    case 22:
      return Pico_Deck_Display23_ON[x][y];
    case 23:
      return Pico_Deck_Display24_ON[x][y];
    default:
      return false;
  }
}

bool getOffPixel(int displayIndex, int x, int y) {
  switch (displayIndex) {
    case 0:
      return Pico_Deck_Display01_OFF[x][y];
    case 1:
      return Pico_Deck_Display02_OFF[x][y];
    case 2:
      return Pico_Deck_Display03_OFF[x][y];
    case 3:
      return Pico_Deck_Display04_OFF[x][y];
    case 4:
      return Pico_Deck_Display05_OFF[x][y];
    case 5:
      return Pico_Deck_Display06_OFF[x][y];
    case 6:
      return Pico_Deck_Display07_OFF[x][y];
    case 7:
      return Pico_Deck_Display08_OFF[x][y];
    case 8:
      return Pico_Deck_Display09_OFF[x][y];
    case 9:
      return Pico_Deck_Display10_OFF[x][y];
    case 10:
      return Pico_Deck_Display11_OFF[x][y];
    case 11:
      return Pico_Deck_Display12_OFF[x][y];
    case 12:
      return Pico_Deck_Display13_OFF[x][y];
    case 13:
      return Pico_Deck_Display14_OFF[x][y];
    case 14:
      return Pico_Deck_Display15_OFF[x][y];
    case 15:
      return Pico_Deck_Display16_OFF[x][y];
    case 16:
      return Pico_Deck_Display17_OFF[x][y];
    case 17:
      return Pico_Deck_Display18_OFF[x][y];
    case 18:
      return Pico_Deck_Display19_OFF[x][y];
    case 19:
      return Pico_Deck_Display20_OFF[x][y];
    case 20:
      return Pico_Deck_Display21_OFF[x][y];
    case 21:
      return Pico_Deck_Display22_OFF[x][y];
    case 22:
      return Pico_Deck_Display23_OFF[x][y];
    case 23:
      return Pico_Deck_Display24_OFF[x][y];
    default:
      return false;
  }
}
