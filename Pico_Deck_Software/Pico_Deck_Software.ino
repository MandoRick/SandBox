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

#define TOUCH_AREA_QTY 44

uint8_t muxAddressPins[NUM_MULTIPLEXERS] = { 10, 11, 12 };
uint8_t multiplexerAddresses[NUM_MULTIPLEXERS] = { 0x71, 0x72, 0x74 };

uint16_t touchAreaBounds[TOUCH_AREA_QTY][4] = {
  // Format: {x1, y1, x2, y2}
  { 890, 900, 1100, 1150 },    // Touch area 0 Display touches
  { 1290, 900, 1500, 1150 },   // Touch area 1
  { 1690, 900, 1900, 1150 },   // Touch area 2
  { 2090, 900, 2300, 1150 },   // Touch area 3
  { 2490, 900, 2700, 1150 },   // Touch area 4
  { 2890, 900, 3100, 1150 },   // Touch area 5
  { 890, 1600, 1100, 1850 },   // Touch area 6 Next Row
  { 1290, 1600, 1500, 1850 },  // Touch area 7
  { 1690, 1600, 1900, 1850 },  // Touch area 8
  { 2090, 1600, 2300, 1850 },  // Touch area 9
  { 2490, 1600, 2700, 1850 },  // Touch area 10
  { 2890, 1600, 3100, 1850 },  // Touch area 11
  { 890, 2300, 1100, 2550 },   // Touch area 12 Next Row
  { 1290, 2300, 1500, 2550 },  // Touch area 13
  { 1690, 2300, 1900, 2550 },  // Touch area 14
  { 2090, 2300, 2300, 2550 },  // Touch area 15
  { 2490, 2300, 2700, 2550 },  // Touch area 16
  { 2890, 2300, 3100, 2550 },  // Touch area 17
  { 890, 3000, 1100, 3250 },   // Touch area 18 Next Row
  { 1290, 3000, 1500, 3250 },  // Touch area 19
  { 1690, 3000, 1900, 3250 },  // Touch area 20
  { 2090, 3000, 2300, 3250 },  // Touch area 21
  { 2490, 3000, 2700, 3250 },  // Touch area 22
  { 2890, 3000, 3100, 3250 },  // Touch area 23
  { 290, 3359, 512, 3682 },    // Touch area 24 Slider 1 touches
  { 290, 3035, 512, 3358 },    // Touch area 25
  { 290, 2711, 512, 3034 },    // Touch area 26
  { 290, 2387, 512, 2710 },    // Touch area 27
  { 290, 2063, 512, 2386 },    // Touch area 28
  { 290, 1739, 512, 2062 },    // Touch area 29
  { 290, 1415, 512, 1738 },    // Touch area 30
  { 290, 1191, 512, 1414 },    // Touch area 31
  { 290, 767, 512, 1090 },     // Touch area 32
  { 290, 442, 512, 766 },      // Touch area 33
  { 3494, 3359, 3725, 3682 },  // Touch area 34 Slider 2 touches
  { 3494, 3035, 3725, 3358 },  // Touch area 35
  { 3494, 2711, 3725, 3034 },  // Touch area 36
  { 3494, 2387, 3725, 2710 },  // Touch area 37
  { 3494, 2063, 3725, 2386 },  // Touch area 38
  { 3494, 1739, 3725, 2062 },  // Touch area 39
  { 3494, 1415, 3725, 1738 },  // Touch area 40
  { 3494, 1191, 3725, 1414 },  // Touch area 41
  { 3494, 767, 3725, 1090 },   // Touch area 42
  { 3494, 442, 3725, 766 }     // Touch area 43
};

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
  setupDisplays();
  touch.begin();
  touch.command(SETUP_COMMAND, POWERDOWN_IRQON, ADC_12BIT);
}

void loop() {
  checkTouchAreas();
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
      delay(10);
      TCA9548A(multiplexerAddresses[multiplexer], 8);
    }
  }
  for (int display = 0; display < DISP_QTY_TOTAL; display++) {
    displaySelected[display] = false;
    oldDisplaySelected[display] = false;
  }
}

void renderDisplays() {
  int displayIndex = 0;
  for (int multiplexer = 0; multiplexer < NUM_MULTIPLEXERS; multiplexer++) {
    for (int display = 0; display < DISP_QTY_PER_MUX; display++) {
      TCA9548A(multiplexerAddresses[multiplexer], display);
      u8g2.clearBuffer();
      u8g2.drawStr(60, 0, (String(display + multiplexer * DISP_QTY_PER_MUX)).c_str());
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
      displayIndex++;
      TCA9548A(multiplexerAddresses[multiplexer], 8);
    }
  }
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

void (*touchAreaFunctions[TOUCH_AREA_QTY])() = {
  touchAreaFunction00,
  touchAreaFunction01,
  touchAreaFunction02,
  touchAreaFunction03,
  touchAreaFunction04,
  touchAreaFunction05,
  touchAreaFunction06,
  touchAreaFunction07,
  touchAreaFunction08,
  touchAreaFunction09,
  touchAreaFunction10,
  touchAreaFunction11,
  touchAreaFunction12,
  touchAreaFunction13,
  touchAreaFunction14,
  touchAreaFunction15,
  touchAreaFunction16,
  touchAreaFunction17,
  touchAreaFunction18,
  touchAreaFunction19,
  touchAreaFunction20,
  touchAreaFunction21,
  touchAreaFunction22,
  touchAreaFunction23,
  touchAreaFunction24,
  touchAreaFunction25,
  touchAreaFunction26,
  touchAreaFunction27,
  touchAreaFunction28,
  touchAreaFunction29,
  touchAreaFunction30,
  touchAreaFunction31,
  touchAreaFunction32,
  touchAreaFunction33,
  touchAreaFunction34,
  touchAreaFunction35,
  touchAreaFunction36,
  touchAreaFunction37,
  touchAreaFunction38,
  touchAreaFunction39,
  touchAreaFunction40,
  touchAreaFunction41,
  touchAreaFunction42,
  touchAreaFunction43,
};

// Read touch input and determine which touch area (if any) is being touched
void checkTouchAreas() {
  uint16_t x, y, z1, z2;
  if (touch.read_touch(&x, &y, &z1, &z2)) {
    for (int i = 0; i < TOUCH_AREA_QTY; i++) {
      if (x >= touchAreaBounds[i][0] && x <= touchAreaBounds[i][2] && y >= touchAreaBounds[i][1] && y <= touchAreaBounds[i][3]) {
        // Execute function associated with the touched area
        touchAreaFunctions[i]();
        break;  // Exit loop after handling the touch event
      }
    }
  }
}

// Touch area jobs for 24 displays
void touchAreaFunction00() {
  Serial.println("Area 00 touched");
  displaySelected[0] = !displaySelected[0];
  Keyboard.print("Touch area 0");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction01() {
  Serial.println("Area 01 touched");
  displaySelected[1] = !displaySelected[1];
  Keyboard.print("Touch area 1");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction02() {
  Serial.println("Area 02 touched");
  displaySelected[2] = !displaySelected[2];
  Keyboard.print("Touch area 2");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction03() {
  Serial.println("Area 03 touched");
  displaySelected[3] = !displaySelected[3];
  Keyboard.print("Touch area 3");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction04() {
  Serial.println("Area 04 touched");
  displaySelected[4] = !displaySelected[4];
  Keyboard.print("Touch area 4");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction05() {
  Serial.println("Area 05 touched");
  displaySelected[5] = !displaySelected[5];
  Keyboard.print("Touch area 5");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction06() {
  Serial.println("Area 06 touched");
  displaySelected[6] = !displaySelected[6];
  Keyboard.print("Touch area 6");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction07() {
  Serial.println("Area 07 touched");
  displaySelected[7] = !displaySelected[7];
  Keyboard.print("Touch area 7");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction08() {
  Serial.println("Area 08 touched");
  displaySelected[8] = !displaySelected[8];
  Keyboard.print("Touch area 8");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction09() {
  Serial.println("Area 09 touched");
  displaySelected[9] = !displaySelected[9];
  Keyboard.print("Touch area 9");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction10() {
  Serial.println("Area 10 touched");
  displaySelected[10] = !displaySelected[10];
  Keyboard.print("Touch area 10");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction11() {
  Serial.println("Area 11 touched");
  displaySelected[11] = !displaySelected[11];
  Keyboard.print("Touch area 11");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction12() {
  Serial.println("Area 12 touched");
  displaySelected[12] = !displaySelected[12];
  Keyboard.print("Touch area 12");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction13() {
  Serial.println("Area 13 touched");
  displaySelected[13] = !displaySelected[13];
  Keyboard.print("Touch area 13");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction14() {
  Serial.println("Area 14 touched");
  displaySelected[14] = !displaySelected[14];
  Keyboard.print("Touch area 14");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction15() {
  Serial.println("Area 15 touched");
  displaySelected[15] = !displaySelected[15];
  Keyboard.print("Touch area 15");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction16() {
  Serial.println("Area 16 touched");
  displaySelected[16] = !displaySelected[16];
  Keyboard.print("Touch area 16");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction17() {
  Serial.println("Area 17 touched");
  displaySelected[17] = !displaySelected[17];
  Keyboard.print("Touch area 17");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction18() {
  Serial.println("Area 18 touched");
  displaySelected[18] = !displaySelected[18];
  Keyboard.print("Touch area 18");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction19() {
  Serial.println("Area 19 touched");
  displaySelected[19] = !displaySelected[19];
  Keyboard.print("Touch area 19");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction20() {
  Serial.println("Area 20 touched");
  displaySelected[20] = !displaySelected[20];
  Keyboard.print("Touch area 20");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction21() {
  Serial.println("Area 21 touched");
  displaySelected[21] = !displaySelected[21];
  Keyboard.print("Touch area 21");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction22() {
  Serial.println("Area 22 touched");
  displaySelected[22] = !displaySelected[22];
  Keyboard.print("Touch area 22");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction23() {
  Serial.println("Area 23 touched");
  displaySelected[23] = !displaySelected[23];
  Keyboard.print("Touch area 23");
  Keyboard.write(KEY_RETURN);
}

// Touch area jobs for first slider
void touchAreaFunction24() {
  Serial.println("Area 24 touched");
  Keyboard.print("Touch area 24");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction25() {
  Serial.println("Area 25 touched");
  Keyboard.print("Touch area 25");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction26() {
  Serial.println("Area 26 touched");
  Keyboard.print("Touch area 26");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction27() {
  Serial.println("Area 27 touched");
  Keyboard.print("Touch area 27");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction28() {
  Serial.println("Area 28 touched");
  Keyboard.print("Touch area 28");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction29() {
  Serial.println("Area 29 touched");
  Keyboard.print("Touch area 29");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction30() {
  Serial.println("Area 30 touched");
  Keyboard.print("Touch area 30");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction31() {
  Serial.println("Area 31 touched");
  Keyboard.print("Touch area 31");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction32() {
  Serial.println("Area 32 touched");
  Keyboard.print("Touch area 32");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction33() {
  Serial.println("Area 33 touched");
  Keyboard.print("Touch area 33");
  Keyboard.write(KEY_RETURN);
}

// Touch area jobs for second slider
void touchAreaFunction34() {
  Serial.println("Area 34 touched");
  Keyboard.print("Touch area 34");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction35() {
  Serial.println("Area 35 touched");
  Keyboard.print("Touch area 35");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction36() {
  Serial.println("Area 36 touched");
  Keyboard.print("Touch area 36");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction37() {
  Serial.println("Area 37 touched");
  Keyboard.print("Touch area 37");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction38() {
  Serial.println("Area 38 touched");
  Keyboard.print("Touch area 38");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction39() {
  Serial.println("Area 39 touched");
  Keyboard.print("Touch area 39");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction40() {
  Serial.println("Area 40 touched");
  Keyboard.print("Touch area 40");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction41() {
  Serial.println("Area 41 touched");
  Keyboard.print("Touch area 41");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction42() {
  Serial.println("Area 42 touched");
  Keyboard.print("Touch area 42");
  Keyboard.write(KEY_RETURN);
}

void touchAreaFunction43() {
  Serial.println("Area 43 touched");
  Keyboard.print("Touch area 43");
  Keyboard.write(KEY_RETURN);
}
