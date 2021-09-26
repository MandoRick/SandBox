/* -------------------
   created by JediRick
   -------------------
   boolean (8 bit) true/false
   byte (8 bit) - unsigned number from 0-255
   char (8 bit) - signed number from -128 to 127
   unsigned char (8 bit) - same as byte
   word (16 bit) - unsigned number from 0-65535
   unsigned int (16 bit)- the same as word
   int (16 bit) - signed number from -32768 to 32767
   unsigned long (32 bit) - unsigned number from 0-4,294,967,295  Usually for millis
   long (32 bit) - signed number from -2,147,483,648 to 2,147,483,647
   float (32 bit) - signed number from -3.4028235E38 to 3.4028235E38  Floating point not native so avoid if you can
*/

// Wire


#include <Wire.h> // This library allows you to communicate with I2C devices.
#include <ds3231.h> //RTC library
ts t; //ts is a struct findable in ds3231.h
const int MPU_ADDR = 0x68; // I2C address of the MPU-6050. If AD0 pin is set to HIGH, the I2C address will be 0x69.
int16_t accelerometer_x, accelerometer_y, accelerometer_z; // variables for accelerometer raw data
int16_t gyro_x, gyro_y, gyro_z; // variables for gyro raw data
int16_t temperature; // variables for temperature data
char tmp_str[7]; // temporary variable used in convert function
char* convert_int16_to_str(int16_t i) { // converts int16 to string. Moreover, resulting strings will have the same length in the debug monitor.
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

const String p_project = "xxx";
const uint8_t version_hi = 0;
const uint8_t version_lo = 1;


void versionPrint(void) {
  Serial.print("RicksWorx: ");
  Serial.println(p_project);
  Serial.print("Version: ");
  Serial.print(version_hi);
  Serial.print('.');
  Serial.println(version_lo);
}


void setup() {
  Serial.begin(115200);
  delay(2000);
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  versionPrint();
  Wire1.begin();
  Wire1.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire1.write(0x6B); // PWR_MGMT_1 register
  Wire1.write(0); // set to zero (wakes up the MPU-6050)
  Wire1.endTransmission(true);
}

void setup1() {
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();
  DS3231_init(DS3231_INTCN);
}

void loop() {
  Wire1.beginTransmission(MPU_ADDR);
  Wire1.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire1.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire1.requestFrom(MPU_ADDR, 7 * 2, true); // request a total of 7*2=14 registers

  // "Wire1.read()<<8 | Wire1.read();" means two registers are read and stored in the same variable
  accelerometer_x = Wire1.read() << 8 | Wire1.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  accelerometer_y = Wire1.read() << 8 | Wire1.read(); // reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  accelerometer_z = Wire1.read() << 8 | Wire1.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire1.read() << 8 | Wire1.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  gyro_x = Wire1.read() << 8 | Wire1.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  gyro_y = Wire1.read() << 8 | Wire1.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  gyro_z = Wire1.read() << 8 | Wire1.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)

  // print out data
  Serial.print("aX = "); Serial.println(convert_int16_to_str(accelerometer_x));
  Serial.print("aY = "); Serial.println(convert_int16_to_str(accelerometer_y));
  Serial.print("aZ = "); Serial.println(convert_int16_to_str(accelerometer_z));
  // the following equation was taken from the documentation [MPU-6000/MPU-6050 Register Map and Description, p.30]
  Serial.print("tmp = "); Serial.println(temperature / 340.00 + 36.53);
  Serial.print("gX = "); Serial.println(convert_int16_to_str(gyro_x));
  Serial.print("gY = "); Serial.println(convert_int16_to_str(gyro_y));
  Serial.print("gZ = "); Serial.println(convert_int16_to_str(gyro_z));
  Serial.println();

  // delay
  delay(110);
}

void loop1() {
  DS3231_get(&t); //get the value and pass to the function the pointer to t, that make an lower memory fingerprint and faster execution than use return
  //DS3231_get() will use the pointer of t to directly change t value (faster, lower memory used)
  Serial.print("Day : ");
  Serial.print(t.mon); //just print some data
  Serial.print("/");
  Serial.print(t.mday);
  Serial.print("/");
  Serial.println(t.year);
  Serial.print("Hour : ");
  Serial.print(t.hour);
  Serial.print(":");
  Serial.print(t.min);
  Serial.print(".");
  Serial.println(t.sec);
#ifdef CONFIG_UNIXTIME
  Serial.print("Timestamp : ");
  Serial.print(t.unixtime);
#endif
  Serial.println();
  delay(100);
}
