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
#include <GyverBME280.h>

GyverOLED<SSH1106_128x64> oled;
GyverBME280 bme;

#define DATA_DIGITS (24)

uint32_t DataOld[DATA_DIGITS];
uint32_t DataNew[DATA_DIGITS];

void setup() {
  Serial.begin(115200);
  oled.init();
  Wire.setClock(800000L);  // макс. 800'000
  oled.clear();
  oled.update();
  bme.begin();
  delay(2000);
  uint32_t pressure1 = bme.readPressure();
  pressure1 = pressure1 - 80000;
  for (uint8_t i = 0; i < DATA_DIGITS; i++) {
    DataOld[i] = pressure1;
  }
  for (uint8_t i = 0; i < DATA_DIGITS; i++) {
    DataNew[i] = DataOld[i];
  }
}

void loop() {
  collect_data();
  parse_data();
  print_data();
  //delay(2000);
  delay(1800000);
}

void collect_data(void) {
  uint32_t pressure1 = bme.readPressure();
  Serial.println(pressure1);
  pressure1 = pressure1 - 80000;
  DataNew[23] = pressure1;
  for (uint8_t i = 0; i < DATA_DIGITS - 1; i++) {
    DataNew[i] = DataOld[i + 1];
  }
}

void parse_data(void) {
  for (uint8_t i = 0; i < DATA_DIGITS; i++) {
    DataOld[i] = DataNew[i];
  }
}

void print_data(void) {
  for (uint8_t u = 0; u < DATA_DIGITS; u++) {
    Serial.print(DataNew[u]);
    Serial.print(", ");
  }
  Serial.println(" ");
  //Serial.println(bme.readTemperature());
  oled.clear();
  for (uint8_t j = 0; j < DATA_DIGITS - 1; j++) {
    uint16_t x1 = 4 + (j * 5);
    uint16_t x2 = 9 + (j * 5);
    uint16_t y1 = map(DataNew[j], 10000, 100000, 5, 60);
    uint16_t y2 = map(DataNew[j + 1], 10000, 100000, 5, 60);
    oled.line(x1, y1, x2, y2, 1);
  }
  oled.update();
}
