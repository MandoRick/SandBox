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
#include <math.h>
#include <AlienMicroRTClibrary.h>
#include <PointsOnCircle.h>
#include <sh1107.h>
#include "data.h"

#define DISP_W 128
#define DISP_H 128
#define OLED_CLOCK_SPEED 1899999ul
#define RTC_CLOCK_SPEED 400000ul

#define CENTER_X 64
#define CENTER_Y 64

#define WHITE 1
#define BLACK 0

#define DIGIT_QTY 6

#define SDA0_PIN 4
#define SCL0_PIN 5
#define SDA1_PIN 6
#define SCL1_PIN 7

char daysOfTheWeek[7][4] = { "SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT" };

int numberArray[DIGIT_QTY];

struct CurrentTime {
  int currentSeconds;
  int currentMinutes;
  int currentHours;
};

RTC_DS3231 rtc;
SH1107 display;
CurrentTime currenttime;
PointsOnCircle circlePoints;

void setup() {
  Serial.begin(115200);
  delay(500);
  setupRTC();
  delay(500);
  setupDisplay();
  delay(5000);
}

void loop() {
  calculateTime();
  drawDisplay();
}

void setupRTC() {
  Wire.setSDA(SDA0_PIN);
  Wire.setSCL(SCL0_PIN);
  Wire.begin();
  Wire.setClock(RTC_CLOCK_SPEED);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void setupDisplay() {
  Wire1.setSDA(SDA1_PIN);
  Wire1.setSCL(SCL1_PIN);
  Wire1.begin();
  display.begin(Wire1);
  Wire1.setClock(OLED_CLOCK_SPEED);
  clearDisplay();
  display.display();
}

void calculateTime() {
  //timer.currentMillis = millis();
  DateTime now = rtc.now();
  currenttime.currentSeconds = now.second();
  currenttime.currentMinutes = now.minute();
  currenttime.currentHours = now.hour();
  currenttime.currentSeconds = currenttime.currentSeconds + 1;
  currenttime.currentMinutes = currenttime.currentMinutes + 1;
  if (currenttime.currentHours > 12) {
    currenttime.currentHours = currenttime.currentHours - 12;
  }
  if (currenttime.currentHours == 0) {
    currenttime.currentHours = 12;
  }
  if (currenttime.currentMinutes == 60) {
    currenttime.currentMinutes = 0;
  }
  if (currenttime.currentSeconds == 60) {
    currenttime.currentSeconds = 0;
  }
  numberArray[0] = currenttime.currentHours / 10;
  numberArray[1] = currenttime.currentHours % 10;
  numberArray[2] = currenttime.currentMinutes / 10;
  numberArray[3] = currenttime.currentMinutes % 10;
  numberArray[4] = currenttime.currentSeconds / 10;
  numberArray[5] = currenttime.currentSeconds % 10;
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
  int x = 0;
  // Draw digitaltime Hours onto the display
  for (int i = 0; i < 2; i++) {
    drawDigit(x, 0, numberArray[i]);
    x = x + 10;
  }
  // Draw digitaltime Minutes onto the display
  x = 25;
  for (int i = 2; i < 4; i++) {
    drawDigit(x, 0, numberArray[i]);
    x = x + 10;
  }
  // Draw digitaltime Seconds onto the display
  x = 56;
  for (int i = 4; i < 6; i++) {
    drawDigit(x, 59, numberArray[i]);
    x = x + 10;
  }
  // Draw dots onto the display
  if (numberArray[5] % 2 != 0) {
    display.drawPixel(20, 3, 1);
    display.drawPixel(20, 7, 1);
  }
}

void drawAnalogueTime() {
  drawAnalogueSeconds();
  drawAnalogueMinutes();
  drawAnalogueHours();
}

void drawAnalogueSeconds() {
  for (float i = -96; i < ((currenttime.currentSeconds + 1) * 6) - 96; i += 6) {
    int x1, y1, x2, y2, x3, y3, x4, y4, angle_degrees, radius;
    radius = 45;
    angle_degrees = i;
    PointsOnCircle::Point result1 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x1 = result1.x;
    y1 = result1.y;
    angle_degrees = i + 2;
    PointsOnCircle::Point result2 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x2 = result2.x;
    y2 = result2.y;
    radius = 55;
    angle_degrees = i;
    PointsOnCircle::Point result3 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x3 = result3.x;
    y3 = result3.y;
    angle_degrees = i + 2;
    PointsOnCircle::Point result4 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x4 = result4.x;
    y4 = result4.y;
    display.drawLine(x1, y1, x2, y2, WHITE);
    display.drawLine(x3, y3, x4, y4, WHITE);
    display.drawLine(x1, y1, x3, y3, WHITE);
    display.drawLine(x2, y2, x4, y4, WHITE);
  }
}

void drawAnalogueMinutes() {
  for (float i = -96; i < ((currenttime.currentMinutes + 1) * 6) - 96; i += 6) {
    int x1, y1, x2, y2, x3, y3, x4, y4, angle_degrees, radius;
    radius = 30;
    angle_degrees = i;
    PointsOnCircle::Point result1 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x1 = result1.x;
    y1 = result1.y;
    angle_degrees = i + 2;
    PointsOnCircle::Point result2 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x2 = result2.x;
    y2 = result2.y;
    radius = 40;
    angle_degrees = i;
    PointsOnCircle::Point result3 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x3 = result3.x;
    y3 = result3.y;
    angle_degrees = i + 2;
    PointsOnCircle::Point result4 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x4 = result4.x;
    y4 = result4.y;
    display.drawLine(x1, y1, x2, y2, WHITE);
    display.drawLine(x3, y3, x4, y4, WHITE);
    display.drawLine(x1, y1, x3, y3, WHITE);
    display.drawLine(x2, y2, x4, y4, WHITE);
  }
}

void drawAnalogueHours() {
  for (float i = -90; i < (currenttime.currentHours * 30) - 90; i += 30) {
    int x1, y1, x2, y2, x3, y3, x4, y4, angle_degrees, radius;
    radius = 15;
    angle_degrees = i;
    PointsOnCircle::Point result1 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x1 = result1.x;
    y1 = result1.y;
    angle_degrees = i + 20;
    PointsOnCircle::Point result2 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x2 = result2.x;
    y2 = result2.y;
    radius = 25;
    angle_degrees = i;
    PointsOnCircle::Point result3 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
    x3 = result3.x;
    y3 = result3.y;
    angle_degrees = i + 20;
    PointsOnCircle::Point result4 = circlePoints.calculate_point_on_circle(radius, CENTER_X, CENTER_Y, angle_degrees);
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
