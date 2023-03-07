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

#include <GyverOLED.h>
GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

#define dispWidth (128)
#define dispHeight (62)

uint8_t gameBoard[dispWidth][dispHeight];
uint8_t antX = dispWidth / 2;
uint8_t antY = dispHeight / 2;
uint8_t andDirection = 0;  //0 - north, 1 - east, 2 - south, 3 - west

void setup() {
  Serial.begin(115200);
  oled.init();
  Wire.setClock(3200000);
  oled.clear();
  oled.update();
  initiateBoard();
  gameBoard[antX][antY] = 1;
}

void loop() {
  moveAnt();
  drawBoard();
  if (antX >= 128 || antY >= 64 || antX == 0 || antY == 0) {
    antX = 64;
    antY = 32;
    initiateBoard();
  }
}

void initiateBoard() {
  for (int i = 0; i < dispWidth; i++) {
    for (int j = 0; j < dispHeight; j++) {
      gameBoard[i][j] = 0;
    }
  }
}

void moveAnt() {
  if (andDirection == 0) {
    antY = antY - 1;
  }
  if (andDirection == 1) {
    antX = antX + 1;
  }
  if (andDirection == 2) {
    antY = antY + 1;
  }
  if (andDirection == 3) {
    antX = antX - 1;
  }
  if (gameBoard[antX][antY] == 1) {
    gameBoard[antX][antY] = 0;
    andDirection = andDirection + 1;
    if (andDirection == 4) {
      andDirection = 0;
    }
  } else {
    gameBoard[antX][antY] = 1;
    andDirection = andDirection - 1;
    if (andDirection == 255) {
      andDirection = 3;
    }
  }
}

void drawBoard() {
  oled.clear();
  for (int i = 0; i < dispWidth; i++) {
    for (int j = 0; j < dispHeight; j++) {
      if (gameBoard[i][j] == 1) {
        oled.dot(i, j);
      }
    }
  }
  oled.update();
}
