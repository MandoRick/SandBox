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

/**
 * globals
 */

const byte DATA_SIZE = 2;
String data[DATA_SIZE] = {
    "12345,123.4,50.6,12345#",
    "4.10,4.15,4.13,4.05,4.18,0#"
};

/**
 * setup & loop
 */

void setup()
{
    Serial.begin(115200);
    Wire.begin(8);
    Wire.onRequest(requestEvent); // data request from Master

    Serial.println("I2C Slave ready!");
}

void loop()
{
    delay(50);
}

/**
 * help functions
 */

void requestEvent()
{
    static byte req_number = 0;

    Wire.write(reinterpret_cast<const unsigned char*>(data[req_number].c_str()),
               data[req_number].length());

    req_number = (req_number + 1) % DATA_SIZE;
}