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

#include <sh1107.h>
#include "numbers.h"

#define DISP_W 128
#define DISP_H 128

#define WHITE 1
#define BLACK 0

#define OLED_I2C_SPEED 1999999ul  // Tested on Pico, Lower speed, no artifacts, 40+ frames per second
//#define OLED_I2C_SPEED 2999999ul  // Tested on Pico, Higher speed, some artifacts, 50+ frames per second

double vectors[8][3] = {
  { 30, 30, 30 },    // Front top-right
  { -30, 30, 30 },   // Front top-left
  { -30, -30, 30 },  // Front bottom-left
  { 30, -30, 30 },   // Front bottom-right
  { 30, 30, -30 },   // Back top-right
  { -30, 30, -30 },  // Back top-left
  { -30, -30, -30 }, // Back bottom-left
  { 30, -30, -30 }   // Back bottom-right
};

double perspective = 100.0f;
int deltaX, deltaY, deltaZ, iter = 0;
uint32_t startTime;
uint32_t endTime;
uint32_t loopTime;
uint32_t framesPerSecond;

SH1107 display;

void setup() {
  display.begin();
  Wire.setClock(OLED_I2C_SPEED);
}

void loop() {
  startTime = millis();
  clearDisplay();
  drawVectors();
  if (iter == 0) {
    deltaX = random(7) - 3;
    deltaY = random(7) - 3;
    deltaZ = random(7) - 3;
    iter = random(250) + 5;
  }
  rotateX(deltaX);
  rotateY(deltaY);
  rotateZ(deltaZ);
  iter--;
  // Draw the first digit of FPS
  int digit1 = framesPerSecond / 100;
  drawDigit(0, 0, digit1);

  // Draw the second digit of FPS
  int digit2 = (framesPerSecond / 10) % 10;
  drawDigit(8, 0, digit2);

  // Draw the third digit of FPS (if needed)
  int digit3 = framesPerSecond % 10;
  drawDigit(16, 0, digit3);
  display.display();
  endTime = millis();
  loopTime = endTime - startTime;
  framesPerSecond = 1000 / loopTime;
}

int translateX(double x, double z) {
  return (int)((x + 64) + (z * (x / perspective)));
}

int translateY(double y, double z) {
  return (int)((y + 64) + (z * (y / perspective)));
}

void rotateX(int angle) {
  double rad, cosa, sina, Yn, Zn;
  rad = angle * PI / 180;
  cosa = cos(rad);
  sina = sin(rad);
  for (int i = 0; i < 8; i++) {
    Yn = (vectors[i][1] * cosa) - (vectors[i][2] * sina);
    Zn = (vectors[i][1] * sina) + (vectors[i][2] * cosa);
    vectors[i][1] = Yn;
    vectors[i][2] = Zn;
  }
}

void rotateY(int angle) {
  double rad, cosa, sina, Xn, Zn;
  rad = angle * PI / 180;
  cosa = cos(rad);
  sina = sin(rad);
  for (int i = 0; i < 8; i++) {
    Xn = (vectors[i][0] * cosa) - (vectors[i][2] * sina);
    Zn = (vectors[i][0] * sina) + (vectors[i][2] * cosa);
    vectors[i][0] = Xn;
    vectors[i][2] = Zn;
  }
}

void rotateZ(int angle) {
  double rad, cosa, sina, Xn, Yn;
  rad = angle * PI / 180;
  cosa = cos(rad);
  sina = sin(rad);
  for (int i = 0; i < 8; i++) {
    Xn = (vectors[i][0] * cosa) - (vectors[i][1] * sina);
    Yn = (vectors[i][0] * sina) + (vectors[i][1] * cosa);
    vectors[i][0] = Xn;
    vectors[i][1] = Yn;
  }
}

void drawVectors() {
  display.drawLine(translateX(vectors[0][0], vectors[0][2]), translateY(vectors[0][1], vectors[0][2]), translateX(vectors[1][0], vectors[1][2]), translateY(vectors[1][1], vectors[1][2]), WHITE);
  display.drawLine(translateX(vectors[1][0], vectors[1][2]), translateY(vectors[1][1], vectors[1][2]), translateX(vectors[2][0], vectors[2][2]), translateY(vectors[2][1], vectors[2][2]), WHITE);
  display.drawLine(translateX(vectors[2][0], vectors[2][2]), translateY(vectors[2][1], vectors[2][2]), translateX(vectors[3][0], vectors[3][2]), translateY(vectors[3][1], vectors[3][2]), WHITE);
  display.drawLine(translateX(vectors[3][0], vectors[3][2]), translateY(vectors[3][1], vectors[3][2]), translateX(vectors[0][0], vectors[0][2]), translateY(vectors[0][1], vectors[0][2]), WHITE);
  display.drawLine(translateX(vectors[4][0], vectors[4][2]), translateY(vectors[4][1], vectors[4][2]), translateX(vectors[5][0], vectors[5][2]), translateY(vectors[5][1], vectors[5][2]), WHITE);
  display.drawLine(translateX(vectors[5][0], vectors[5][2]), translateY(vectors[5][1], vectors[5][2]), translateX(vectors[6][0], vectors[6][2]), translateY(vectors[6][1], vectors[6][2]), WHITE);
  display.drawLine(translateX(vectors[6][0], vectors[6][2]), translateY(vectors[6][1], vectors[6][2]), translateX(vectors[7][0], vectors[7][2]), translateY(vectors[7][1], vectors[7][2]), WHITE);
  display.drawLine(translateX(vectors[7][0], vectors[7][2]), translateY(vectors[7][1], vectors[7][2]), translateX(vectors[4][0], vectors[4][2]), translateY(vectors[4][1], vectors[4][2]), WHITE);
  display.drawLine(translateX(vectors[0][0], vectors[0][2]), translateY(vectors[0][1], vectors[0][2]), translateX(vectors[4][0], vectors[4][2]), translateY(vectors[4][1], vectors[4][2]), WHITE);
  display.drawLine(translateX(vectors[1][0], vectors[1][2]), translateY(vectors[1][1], vectors[1][2]), translateX(vectors[5][0], vectors[5][2]), translateY(vectors[5][1], vectors[5][2]), WHITE);
  display.drawLine(translateX(vectors[2][0], vectors[2][2]), translateY(vectors[2][1], vectors[2][2]), translateX(vectors[6][0], vectors[6][2]), translateY(vectors[6][1], vectors[6][2]), WHITE);
  display.drawLine(translateX(vectors[3][0], vectors[3][2]), translateY(vectors[3][1], vectors[3][2]), translateX(vectors[7][0], vectors[7][2]), translateY(vectors[7][1], vectors[7][2]), WHITE);
}

void clearDisplay() {
  for (int x = 0; x < DISP_W; x++) {
    for (int y = 0; y < DISP_H; y++) {
      display.drawPixel(x, y, BLACK);
    }
  }
}

void drawDigit(int x, int y, int digit) {
  const int segmentX[7] PROGMEM = { 1, 1, 1, 1, 1, 5, 5 };
  const int segmentY[7] PROGMEM = { 1, 5, 9, 1, 5, 1, 5 };
  const int segmentEndX[7] PROGMEM = { 5, 5, 5, 1, 1, 5, 5 };
  const int segmentEndY[7] PROGMEM = { 1, 5, 9, 5, 9, 5, 9 };
  for (int i = 0; i < 7; i++) {
    if (pgm_read_byte(&NUMBERS[digit][i])) {
      display.drawLine(
        x + pgm_read_byte(&segmentX[i]),
        y + pgm_read_byte(&segmentY[i]),
        x + pgm_read_byte(&segmentEndX[i]),
        y + pgm_read_byte(&segmentEndY[i]),
        WHITE);
    }
  }
}