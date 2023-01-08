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
#include <GyverFIFO.h>

GyverFIFO<uint32_t, 16> fifoBuff0;
GyverFIFO<uint32_t, 16> fifoBuff1;
GyverFIFO<uint32_t, 16> fifoBuff2;

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

int32_t countNum[3] = { 0, 30000, 0 };
int32_t buffNum[3] = { 0, 0, 0 };

void setup() {
  Serial.begin(115200);
}

void loop() {
  countNum[0]++;
  countNum[1]--;
  countNum[2]--;
  delay(10);
  Serial.println("number1: " + (String)countNum[0]);
  Serial.println("number2: " + (String)countNum[1]);
  Serial.println("number3: " + (String)countNum[2]);
  fifoBuff0.write(countNum[0]);
  fifoBuff1.write(countNum[1]);
  fifoBuff2.write(countNum[2]);
}

void setup1() {
  oled.init();
  oled.clear();
  oled.update();
}

void loop1() {
  while (fifoBuff0.available()) {
    buffNum[0] = fifoBuff0.read();
  }
  while (fifoBuff1.available()) {
    buffNum[1] = fifoBuff1.read();
  }
  while (fifoBuff2.available()) {
    buffNum[2] = fifoBuff2.read();
  }
  oled.clear();
  oled.setScale(2);
  oled.setCursor(16, 2);
  oled.print("X");
  oled.setCursor(60, 2);
  oled.print("Y");
  oled.setCursor(102, 2);
  oled.print("Z");
  oled.roundRect(2, 40, 42, 62, OLED_STROKE);
  oled.roundRect(44, 40, 84, 62, OLED_STROKE);
  oled.roundRect(86, 40, 126, 62, OLED_STROKE);
  oled.setScale(1);
  oled.setCursor(7, 6);
  oled.print(buffNum[0]);
  oled.setCursor(49, 6);
  oled.print(buffNum[1]);
  oled.setCursor(92, 6);
  oled.print(buffNum[2]);
  oled.update();
}