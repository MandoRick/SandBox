/*
  Arduino LSM6DSOX - Simple Gyroscope

  This example reads the gyroscope values from the LSM6DS3
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Nano RP2040 Connect

  created 10 May 2021
  by Arturo Guadalupi

  This example code is in the public domain.
*/

#include <Arduino_LSM6DSOX.h>

void setup() {
  Serial.begin(115200);
 // while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  Serial.print("Gyroscope sample rate = ");
  Serial.print(IMU.gyroscopeSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in g's");
  Serial.println("X\tY\tZ");

}

void loop() {
  float x1, y1, z1;
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x1, y1, z1);
  }
  float x2, y2, z2;
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x2, y2, z2);
  }
  
  Serial.print(x2);
  Serial.print('\t');
  Serial.print(y2);
  Serial.print('\t');
  Serial.println(z2);

  Serial.print(x1);
  Serial.print('\t');
  Serial.print(y1);
  Serial.print('\t');
  Serial.println(z1);

}
