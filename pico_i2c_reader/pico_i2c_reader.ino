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
bool ledState = false;
#define DATA_DIGITS (12)  //how many data digits
#define DATA_AXIS (10)     //variable count
//  tof1, tof2, tof3, tof4, tof5, tof6, tof7, tof8
//uint16_t [0-65,535]
byte dataArray[DATA_AXIS][DATA_DIGITS];
byte DATA_REQ = 0;

//

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();          // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
}

void loop() {
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, true);
  for (int u = 0; u < DATA_AXIS; u++) {
    DATA_REQ = u;
    Wire.beginTransmission(8);
    Wire.write((byte)DATA_REQ);
    Wire.endTransmission();
    Wire.requestFrom(8, 12);  // request 6 bytes from peripheral device #8
    while (Wire.available()) {
      byte wireData = Wire.read();
      Serial.print(wireData);
    }
    Serial.println("");
  }

  digitalWrite(LED_BUILTIN, false);
  delay(5000);
}