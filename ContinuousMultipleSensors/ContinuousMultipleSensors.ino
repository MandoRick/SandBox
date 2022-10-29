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

#include <Arduino.h>
#include <Wire.h>       //I2C
#include <VL53L1X.h>    //TOF

#define myDebug 1     // 1 ON 0 OFF
#if myDebug == 1
#define debugln(x) Serial.println(x)
#define debug(x) Serial.print(x)
#else
#define debugln(x)
#define debug(x)
#endif

const uint8_t SENSOR_COUNT = 6;
const uint8_t XSHUT_PINS[SENSOR_COUNT] = { 10, 11, 12, 13, 14, 15 };

VL53L1X TOF_SENSORS[SENSOR_COUNT];

void setup() {
  setupSerial();
  setupI2C();
  setupTOF();
}

void loop() {
  updateTOF();
}

void setupSerial() {
  Serial.begin(115200);
  //while (!Serial) {}
  delay(5000);
  debugln("SERIAL SETUP!");
  delay(2000);
}

void setupI2C() {
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
}

void setupTOF() {
  // Disable/reset all TOF_SENSORS by driving their XSHUT pins low.
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)  {
    pinMode(XSHUT_PINS[i], OUTPUT);
    digitalWrite(XSHUT_PINS[i], LOW);
  }
  // Enable, initialize, and start each sensor, one by one.
  for (uint8_t i = 0; i < SENSOR_COUNT; i++)  {
    // Stop driving this sensor's XSHUT low. This should allow the carrier
    // board to pull it high. (We do NOT want to drive XSHUT high since it is
    // not level shifted.) Then wait a bit for the sensor to start up.
    pinMode(XSHUT_PINS[i], INPUT);
    delay(10);
    TOF_SENSORS[i].setTimeout(50);
    if (!TOF_SENSORS[i].init())    {
      debug("TOF FAILURE: ");
      debugln(i);
     //while (1);
    }
    // Each sensor must have its address changed to a unique value other than
    // the default of 0x29 (except for the last one, which could be left at
    // the default). To make it simple, we'll just count up from 0x2A.
    TOF_SENSORS[i].setAddress(0x2A + i);
    TOF_SENSORS[i].startContinuous(50);
  }
}

void updateTOF() {
  for (uint8_t i = 0; i < SENSOR_COUNT; i++) {
    debug(TOF_SENSORS[i].read());
    if (TOF_SENSORS[i].timeoutOccurred()) {
      debug(" TOF TIMEOUT: ");
      debugln(i);
    }
    debugln('\t');
  }
  debugln();
}
