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

#include <GyverFIFO.h>
#include <Wire.h>
#include "sh1107.h"
#include "data.h"

#define DISP_W 128
#define DISP_H 128
#define OLED_CLOCK_SPEED 1999999ul

#define CENTER_X 64
#define CENTER_Y 64

#define WHITE 1
#define BLACK 0

#define DIGIT_QTY 6

int numberArray[DIGIT_QTY];

int currentSeconds, currentMinutes, currentHours;

SH1107 display;

struct Point {
  int x;
  int y;
};

Point calculate_point_on_circle(int radius, int centerX, int centerY, float angle_degrees) {
  Point point;
  float angle_radians = radians(angle_degrees);
  point.x = centerX + radius * cos(angle_radians);
  point.y = centerY + radius * sin(angle_radians);
  return point;
}

void setup() {
  Serial.begin(115200);
  display.begin();
  Wire.setClock(OLED_CLOCK_SPEED);
  clearDisplay();
  display.display();
  currentSeconds = 1;
  currentMinutes = 1;
  currentHours = 1;
  delay(5000);
}

void loop() {
  calculateTime();
  drawDisplay();
}

void calculateTime() {
  currentSeconds = currentSeconds + 1;
  if (currentSeconds == 61) {
    currentSeconds = 1;
    currentMinutes = currentMinutes + 1;
  }
  if (currentMinutes == 61) {
    currentMinutes = 1;
    currentHours = currentHours + 1;
  }
  if (currentHours == 13) {
    currentHours = 1;
  }

  numberArray[0] = currentHours / 10;
  numberArray[1] = currentHours % 10;
  numberArray[2] = currentMinutes / 10;
  numberArray[3] = currentMinutes % 10;
  numberArray[4] = currentSeconds / 10;
  numberArray[5] = currentSeconds % 10;
}

void drawDisplay() {
  clearDisplay();
  drawDigitalTime();
  drawAnalogueTime();
  display.display();
}

void clearDisplay() {
  for (int x = 0; x < DISP_W; x++) {
    for (int y = 0; y < DISP_H; y++) {
      display.drawPixel(x, y, BLACK);
    }
  }
}

void drawDigitalTime() {
  // Draw playerScoreArray onto the display
  for (int i = 0; i < DIGIT_QTY; i++) {
    drawDigit(i * 10, 0, numberArray[i]);
  }
}

void drawAnalogueTime() {
  drawAnalogueSeconds();
  drawAnalogueMinutes();
  drawAnalogueHours();
}

void drawAnalogueSeconds() {
  for (float i = -90; i < (currentSeconds * 6) - 90; i += 6) {
    int x1, y1, x2, y2, x3, y3, x4, y4, angle_degrees, radius;
    radius = 45;
     angle_degrees = i;
    Point result1 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x1 = result1.x;
    y1 = result1.y;
    angle_degrees = i + 2;
    Point result2 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x2 = result2.x;
    y2 = result2.y;
    radius = 55;
    angle_degrees = i;
    Point result3 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x3 = result3.x;
    y3 = result3.y;
    angle_degrees = i + 2;
    Point result4 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x4 = result4.x;
    y4 = result4.y;
    display.drawLine(x1, y1, x2, y2, WHITE);
    display.drawLine(x3, y3, x4, y4, WHITE);
    display.drawLine(x1, y1, x3, y3, WHITE);
    display.drawLine(x2, y2, x4, y4, WHITE);
  }
}

void drawAnalogueMinutes() {
  for (float i = -90; i < (currentMinutes * 6) - 90; i += 6) {
    int x1, y1, x2, y2, x3, y3, x4, y4, angle_degrees, radius;
    radius = 30;
    angle_degrees = i;
    Point result1 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x1 = result1.x;
    y1 = result1.y;
    angle_degrees = i + 2;
    Point result2 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x2 = result2.x;
    y2 = result2.y;
    radius = 40;
    angle_degrees = i;
    Point result3 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x3 = result3.x;
    y3 = result3.y;
    angle_degrees = i + 2;
    Point result4 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x4 = result4.x;
    y4 = result4.y;
    display.drawLine(x1, y1, x2, y2, WHITE);
    display.drawLine(x3, y3, x4, y4, WHITE);
    display.drawLine(x1, y1, x3, y3, WHITE);
    display.drawLine(x2, y2, x4, y4, WHITE);
  }
}

void drawAnalogueHours() {
  for (float i = -90; i < (currentHours * 30) - 90; i += 30) {
    int x1, y1, x2, y2, x3, y3, x4, y4, angle_degrees, radius;
    radius = 15;
    angle_degrees = i;
    Point result1 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x1 = result1.x;
    y1 = result1.y;
    angle_degrees = i + 20;
    Point result2 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x2 = result2.x;
    y2 = result2.y;
    radius = 25;
    angle_degrees = i;
    Point result3 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x3 = result3.x;
    y3 = result3.y;
    angle_degrees = i + 20;
    Point result4 = calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x4 = result4.x;
    y4 = result4.y;
    display.drawLine(x1, y1, x2, y2, WHITE);
    display.drawLine(x3, y3, x4, y4, WHITE);
    display.drawLine(x1, y1, x3, y3, WHITE);
    display.drawLine(x2, y2, x4, y4, WHITE);
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
