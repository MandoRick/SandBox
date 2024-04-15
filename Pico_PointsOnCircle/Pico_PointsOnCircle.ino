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

/*  Points on a circle Library
    by: JediRick and RefreshMyMind
    wokwi example link: https://wokwi.com/projects/373856384697578497
*/

#include <GyverOLED.h>
#include "data.h"
#include "PointsOnCircle.h"

#define OLED_SPEED (1999999ul)
#define DISP_W 128
#define DISP_H 64
#define CENTER_X 64
#define CENTER_Y 32

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;
PointsOnCircle circlePoints;

void setup() {
  Serial.begin(115200);
  oled.init();
  //Wire.setClock(OLED_SPEED);
}

void loop() {
  drawDisplay();
  delay(250);
}

void drawDisplay() {
  for (float i = 0; i < 360; i += 10) {
    int x1, y1, x2, y2;
    int radius1 = 24;
    int radius2 = 30;
    float angle_degrees = i;
    PointsOnCircle::Point result1 = circlePoints.calculate_point_on_circle(radius1, CENTER_X, CENTER_Y, angle_degrees);
    x1 = result1.x;
    y1 = result1.y;
    PointsOnCircle::Point result2 = circlePoints.calculate_point_on_circle(radius2, CENTER_X, CENTER_Y, angle_degrees);
    x2 = result2.x;
    y2 = result2.y;
    oled.line(x1, y1, x2, y2);
  }
  oled.update();
}
