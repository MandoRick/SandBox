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
#include <Adafruit_LSM6DSOX.h>    //IMU
#include <Wire.h>                 //I2C
#include <VL53L1X.h>              //TOF
#include "ESC.h"                  //ESC

#define myDebug 1     // 1 ON 0 OFF
#if myDebug == 1
#define debugln(x) Serial.println(x)
#define debug(x) Serial.print(x)
#else
#define debugln(x)
#define debug(x)
#endif

#include "ESC.h"
#define LED_PIN (13)      // Pin for the LED 
#define SPEED_MIN (1000)  // Set the Minimum Speed in microseconds
#define SPEED_MAX (2000)  // Set the Minimum Speed in microseconds
#define ESC_PIN1 (20)  //(D08 Yellow)
#define ESC_PIN2 (21)  //(D09 White)
#define ESC_PIN3 (5)   //(D10 Blue)
#define ESC_PIN4 (7)   //(D11 Green)

uint16_t oESC;
float GyroX, GyroY, GyroZ, AccX, AccY, AccZ;

// ESC_Name (ESC PIN, Minimum Value, Maximum Value, Default Speed, Arm Value)
ESC ESC_1 (ESC_PIN1, SPEED_MIN, SPEED_MAX, 500);
ESC ESC_2 (ESC_PIN2, SPEED_MIN, SPEED_MAX, 500);
ESC ESC_3 (ESC_PIN3, SPEED_MIN, SPEED_MAX, 500);
ESC ESC_4 (ESC_PIN4, SPEED_MIN, SPEED_MAX, 500);

Adafruit_LSM6DSOX IMU_DSOX;

void setup() {
  setupSerial();
  setupI2C();
  setupIMU();
  setupESC();
}

void loop() {
  updateESC();
  updateIMU();
}

void setupSerial() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  delay(5000);
  debugln("SERIAL SETUP!");
  delay(2000);
}

void setupIMU() {
  debugln("Adafruit LSM6DSOX test!");
  if (!IMU_DSOX.begin_I2C()) {
    debugln("Failed to find LSM6DSOX chip");
  }
  debugln("LSM6DSOX Found!");
  // IMU_DSOX.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  debug("Accelerometer range set to: ");
  switch (IMU_DSOX.getAccelRange()) {
    case LSM6DS_ACCEL_RANGE_2_G:
      debugln("+-2G");
      break;
    case LSM6DS_ACCEL_RANGE_4_G:
      debugln("+-4G");
      break;
    case LSM6DS_ACCEL_RANGE_8_G:
      debugln("+-8G");
      break;
    case LSM6DS_ACCEL_RANGE_16_G:
      debugln("+-16G");
      break;
  }
  // IMU_DSOX.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  debug("Gyro range set to: ");
  switch (IMU_DSOX.getGyroRange()) {
    case LSM6DS_GYRO_RANGE_125_DPS:
      debugln("125 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_250_DPS:
      debugln("250 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_500_DPS:
      debugln("500 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_1000_DPS:
      debugln("1000 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_2000_DPS:
      debugln("2000 degrees/s");
      break;
    case ISM330DHCX_GYRO_RANGE_4000_DPS:
      break; // unsupported range for the DSOX
  }
  // IMU_DSOX.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  debug("Accelerometer data rate set to: ");
  switch (IMU_DSOX.getAccelDataRate()) {
    case LSM6DS_RATE_SHUTDOWN:
      debugln("0 Hz");
      break;
    case LSM6DS_RATE_12_5_HZ:
      debugln("12.5 Hz");
      break;
    case LSM6DS_RATE_26_HZ:
      debugln("26 Hz");
      break;
    case LSM6DS_RATE_52_HZ:
      debugln("52 Hz");
      break;
    case LSM6DS_RATE_104_HZ:
      debugln("104 Hz");
      break;
    case LSM6DS_RATE_208_HZ:
      debugln("208 Hz");
      break;
    case LSM6DS_RATE_416_HZ:
      debugln("416 Hz");
      break;
    case LSM6DS_RATE_833_HZ:
      debugln("833 Hz");
      break;
    case LSM6DS_RATE_1_66K_HZ:
      debugln("1.66 KHz");
      break;
    case LSM6DS_RATE_3_33K_HZ:
      debugln("3.33 KHz");
      break;
    case LSM6DS_RATE_6_66K_HZ:
      debugln("6.66 KHz");
      break;
  }
  // IMU_DSOX.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  debug("Gyro data rate set to: ");
  switch (IMU_DSOX.getGyroDataRate()) {
    case LSM6DS_RATE_SHUTDOWN:
      debugln("0 Hz");
      break;
    case LSM6DS_RATE_12_5_HZ:
      debugln("12.5 Hz");
      break;
    case LSM6DS_RATE_26_HZ:
      debugln("26 Hz");
      break;
    case LSM6DS_RATE_52_HZ:
      debugln("52 Hz");
      break;
    case LSM6DS_RATE_104_HZ:
      debugln("104 Hz");
      break;
    case LSM6DS_RATE_208_HZ:
      debugln("208 Hz");
      break;
    case LSM6DS_RATE_416_HZ:
      debugln("416 Hz");
      break;
    case LSM6DS_RATE_833_HZ:
      debugln("833 Hz");
      break;
    case LSM6DS_RATE_1_66K_HZ:
      debugln("1.66 KHz");
      break;
    case LSM6DS_RATE_3_33K_HZ:
      debugln("3.33 KHz");
      break;
    case LSM6DS_RATE_6_66K_HZ:
      debugln("6.66 KHz");
      break;
  }
}

void setupI2C() {
  Wire.begin();
}

void setupESC() {
  ESC_1.arm();
  ESC_2.arm();
  ESC_3.arm();
  ESC_4.arm();
  delay(1000);
}

void updateESC() {
  ESC_1.speed(1030);
  ESC_2.speed(1030);
  ESC_3.speed(1030);
  ESC_4.speed(1030);
  //delay(5000);
  ESC_1.speed(1000);
  ESC_2.speed(1000);
  ESC_3.speed(1000);
  ESC_4.speed(1000);
  //delay(5000);
  /*
    ESC_1.speed(1800);
    ESC_2.speed(1800);
    ESC_3.speed(1800);
    ESC_4.speed(1800);
    delay(1000);
    ESC_1.speed(1000);
    ESC_2.speed(1000);
    ESC_3.speed(1000);
    ESC_4.speed(1000);
    delay(5000);
     for (oESC = SPEED_MIN; oESC <= SPEED_MAX; oESC += 1) {  // goes from SPEED_MIN microseconds to SPEED_MAX microseconds
     ESC_1.speed(oESC);
     ESC_2.speed(oESC);
     ESC_3.speed(oESC);
     ESC_4.speed(oESC);
     }
     delay(2000);
     for (oESC = SPEED_MAX; oESC >= SPEED_MIN; oESC -= 1) {  // goes from SPEED_MAX microseconds to SPEED_MIN microseconds
     ESC_1.speed(oESC);
     ESC_2.speed(oESC);
     ESC_3.speed(oESC);
     ESC_4.speed(oESC);
     }
     delay(10000);
    myESC4.speed(1000);
    delay(1000);
    myESC1.speed(2000);
    delay(5000);                                            // waits 10ms for the ESC to reach speed
    myESC1.speed(1000);
    delay(1000);
    myESC2.speed(2000);
    delay(5000);
    myESC2.speed(1000);
    delay(1000);
    myESC3.speed(2000);
    delay(5000);
    myESC3.speed(1000);
    delay(1000);
    myESC4.speed(2000);
    delay(5000);
  */
}

void updateIMU() {


  //  /* Get a new normalized sensor event */
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  IMU_DSOX.getEvent(&accel, &gyro, &temp);

  debug("\t\tTemperature ");
  debug(temp.temperature);
  debugln(" deg C");

  /* Display the results (acceleration is measured in m/s^2) */
  debug("\t\tAccel X: ");
  debug(accel.acceleration.x);
  debug(" \tY: ");
  debug(accel.acceleration.y);
  debug(" \tZ: ");
  debug(accel.acceleration.z);
  debugln(" m/s^2 ");

  /* Display the results (rotation is measured in rad/s) */
  debug("\t\tGyro X: ");
  debug(gyro.gyro.x);
  debug(" \tY: ");
  debug(gyro.gyro.y);
  debug(" \tZ: ");
  debug(gyro.gyro.z);
  debugln(" radians/s ");
  debugln();
  delay(100);

}
