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
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS2MDL.h>   //COMPASS
#include <Adafruit_LSM6DSOX.h>  //IMU
#include <Wire.h>               //I2C
#include <VL53L1X.h>            //TOF
#include "ESC.h"                //ESC

#define SPEED_MIN (1000)  // Set the Minimum Speed in microseconds
#define SPEED_MAX (2000)  // Set the Minimum Speed in microseconds
#define ESC_PIN1 (21)     //(D08 Yellow)
#define ESC_PIN2 (7)     //(D09 White)
#define ESC_PIN3 (20)      //(D10 Blue)
#define ESC_PIN4 (5)      //(D11 Green)
#define DATA_DIGITS (15)  //how many data digits
#define DATA_AXIS (9)     //how many rows of noise buffer and prediction
//  accX, accY, accZ, gyrX, gyrY, gyrZ, magHead, tof1, tof2, tof3, tof4, tof5, tof6, temp, misc
int16_t DataIn[DATA_AXIS][DATA_DIGITS];
int16_t DataOut[DATA_DIGITS];
uint8_t lineCount = 0;
uint16_t oESC;

ESC ESC_1(ESC_PIN1, SPEED_MIN, SPEED_MAX, 500);
ESC ESC_2(ESC_PIN2, SPEED_MIN, SPEED_MAX, 500);
ESC ESC_3(ESC_PIN3, SPEED_MIN, SPEED_MAX, 500);
ESC ESC_4(ESC_PIN4, SPEED_MIN, SPEED_MAX, 500);
Adafruit_LSM6DSOX IMU_DSOX;
Adafruit_LIS2MDL mag = Adafruit_LIS2MDL(12345);

void setup() {
  Serial.begin(115200);
  setupI2C();
  setupESC();
  setupIMU();
  setupCompass();
}

void setupI2C() {
  Wire.begin();
  Serial.println("I2C Setup!");
  delay(1000);
}

void setupESC() {
  ESC_1.arm();
  ESC_2.arm();
  ESC_3.arm();
  ESC_4.arm();
  delay(5000);
  Serial.println("ESC Setup!");
  delay(1000);
}

void setupIMU() {
  Serial.println("Adafruit LSM6DSOX test!");
  if (!IMU_DSOX.begin_I2C()) {
    Serial.println("Failed to find LSM6DSOX chip");
  }
  Serial.println("LSM6DSOX Found!");
  Serial.print("Accelerometer range set to: ");  // IMU_DSOX.setAccelRange(LSM6DS_ACCEL_RANGE_2_G);
  switch (IMU_DSOX.getAccelRange()) {
    case LSM6DS_ACCEL_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case LSM6DS_ACCEL_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case LSM6DS_ACCEL_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case LSM6DS_ACCEL_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  Serial.print("Gyro range set to: ");  // IMU_DSOX.setGyroRange(LSM6DS_GYRO_RANGE_250_DPS );
  switch (IMU_DSOX.getGyroRange()) {
    case LSM6DS_GYRO_RANGE_125_DPS:
      Serial.println("125 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_250_DPS:
      Serial.println("250 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_500_DPS:
      Serial.println("500 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_1000_DPS:
      Serial.println("1000 degrees/s");
      break;
    case LSM6DS_GYRO_RANGE_2000_DPS:
      Serial.println("2000 degrees/s");
      break;
    case ISM330DHCX_GYRO_RANGE_4000_DPS:
      break;  // unsupported range for the DSOX
  }
  Serial.print("Accelerometer data rate set to: ");  // IMU_DSOX.setAccelDataRate(LSM6DS_RATE_12_5_HZ);
  switch (IMU_DSOX.getAccelDataRate()) {
    case LSM6DS_RATE_SHUTDOWN:
      Serial.println("0 Hz");
      break;
    case LSM6DS_RATE_12_5_HZ:
      Serial.println("12.5 Hz");
      break;
    case LSM6DS_RATE_26_HZ:
      Serial.println("26 Hz");
      break;
    case LSM6DS_RATE_52_HZ:
      Serial.println("52 Hz");
      break;
    case LSM6DS_RATE_104_HZ:
      Serial.println("104 Hz");
      break;
    case LSM6DS_RATE_208_HZ:
      Serial.println("208 Hz");
      break;
    case LSM6DS_RATE_416_HZ:
      Serial.println("416 Hz");
      break;
    case LSM6DS_RATE_833_HZ:
      Serial.println("833 Hz");
      break;
    case LSM6DS_RATE_1_66K_HZ:
      Serial.println("1.66 KHz");
      break;
    case LSM6DS_RATE_3_33K_HZ:
      Serial.println("3.33 KHz");
      break;
    case LSM6DS_RATE_6_66K_HZ:
      Serial.println("6.66 KHz");
      break;
  }
  Serial.print("Gyro data rate set to: ");  // IMU_DSOX.setGyroDataRate(LSM6DS_RATE_12_5_HZ);
  switch (IMU_DSOX.getGyroDataRate()) {
    case LSM6DS_RATE_SHUTDOWN:
      Serial.println("0 Hz");
      break;
    case LSM6DS_RATE_12_5_HZ:
      Serial.println("12.5 Hz");
      break;
    case LSM6DS_RATE_26_HZ:
      Serial.println("26 Hz");
      break;
    case LSM6DS_RATE_52_HZ:
      Serial.println("52 Hz");
      break;
    case LSM6DS_RATE_104_HZ:
      Serial.println("104 Hz");
      break;
    case LSM6DS_RATE_208_HZ:
      Serial.println("208 Hz");
      break;
    case LSM6DS_RATE_416_HZ:
      Serial.println("416 Hz");
      break;
    case LSM6DS_RATE_833_HZ:
      Serial.println("833 Hz");
      break;
    case LSM6DS_RATE_1_66K_HZ:
      Serial.println("1.66 KHz");
      break;
    case LSM6DS_RATE_3_33K_HZ:
      Serial.println("3.33 KHz");
      break;
    case LSM6DS_RATE_6_66K_HZ:
      Serial.println("6.66 KHz");
      break;
  }
  Serial.println("IMU Setup!");
  delay(1000);
}

void setupCompass() {
  if (!mag.begin()) {
    Serial.println("Ooops, no LIS2MDL detected ... Check your wiring!"); /* There was a problem detecting the LIS2MDL ... check your connections */
    while (1)
      ;
  }
  Serial.println("Compass Setup!");
  delay(1000);
}

void loop() {
  collect_data();
  parse_data();
  update_ESC();
}

void collect_data(void) {
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  IMU_DSOX.getEvent(&accel, &gyro, &temp);
  DataIn[lineCount][0] = accel.acceleration.x;
  DataIn[lineCount][1] = accel.acceleration.y;
  DataIn[lineCount][2] = accel.acceleration.z;
  DataIn[lineCount][3] = gyro.gyro.x;
  DataIn[lineCount][4] = gyro.gyro.y;
  DataIn[lineCount][5] = gyro.gyro.z;
  DataIn[lineCount][13] = temp.temperature;
  sensors_event_t event;
  mag.getEvent(&event);
  float Pi = 3.14159;
  float heading = (atan2(event.magnetic.y, event.magnetic.x) * 180) / Pi;  // Calculate the angle of the vector y,x
  DataIn[lineCount][6] = heading;
  lineCount++;
  if (lineCount > DATA_AXIS) {
    lineCount = 0;
  }
  for (uint16_t u = 0; u < DATA_DIGITS; u++) {
    Serial.print(DataOut[u]);
    Serial.print(", ");
  }
  Serial.println();
}

void parse_data(void) {
  for (uint16_t i = 0; i < DATA_DIGITS; i++) {
    int16_t averageValue = 0;
    for (uint16_t j = 0; j < DATA_AXIS; j++) {
      averageValue = averageValue + DataIn[j][i];
    }
    averageValue = averageValue / DATA_AXIS;
    DataOut[i] = averageValue;
  }
}

void update_ESC() {
  int16_t lForwardEnginesMux = (DataOut[0] + DataOut[1]) / 2;
  int16_t rForwardEnginesMux = (DataOut[0] + (-DataOut[1])) / 2;
  int16_t lBackwardEnginesMux = ((-DataOut[0]) + DataOut[1]) / 2;
  int16_t rBackwardEnginesMux = ((-DataOut[0]) + (-DataOut[1])) / 2;
  uint16_t lForwardEnginesRPM = map(lForwardEnginesMux, 10, -10, 600, 1280);
  uint16_t rForwardEnginesRPM = map(rForwardEnginesMux, 10, -10, 600, 1280);
  uint16_t lBackwardEnginesRPM = map(lBackwardEnginesMux, 10, -10, 600, 1280);  
  uint16_t rBackwardEnginesRPM = map(rBackwardEnginesMux, 10, -10, 600, 1280);
  ESC_1.speed(lForwardEnginesRPM);
  ESC_2.speed(rForwardEnginesRPM);
  ESC_3.speed(lBackwardEnginesRPM);  
  ESC_4.speed(rBackwardEnginesRPM);
}
