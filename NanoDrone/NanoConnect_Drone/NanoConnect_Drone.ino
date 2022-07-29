/*-data-type------size---------description-----
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
  --------------------------------------------
  camelCase                -  anything that changes
  snake_case               -  variable's that are exclusive in a function
  Snake_Case               -  CLASS/struct exclusave varables/functions
  iNVERTEDcAMELcASE        -  outside code that is being accessed [database]
  SNake_CAse               -  duplicate varables inside the case function [frequently used in library names]
  ALL_CAPS                 -  const varable names or defines
  -------------by-jediRick--------------------
*/


#include <Wire.h>  //i2c
#include <GyverOLED.h> //display
#include <Adafruit_GPS.h> //gps
#include <Arduino_LSM6DSOX.h> //IMU

#define serialGps Serial1
#define serialMonitor Serial
#define GPSECHO (true)
#define displayRefreshDelay (1110)
#define gpsRefreshDelay (2000)


Adafruit_GPS GPS(&serialGps);
GyverOLED<SSH1106_128x64> oled;

int32_t gpsHour, gpsMin, gpsSec, gpsDay, gpsMon, gpsYear, gpsFix, gpsQual, gpsLatitude, gpsLat, gpsLongitude, gpsLon, gpsSpeed, gpsAngle, gpsAltitude, gpsSat , gpsAntenna;
uint32_t timer1, timer2 = millis();
int imuTemp;
float imuAccX, imuAccY, imuAccZ, imuGyroX, imuGyroY, imuGyroZ;
volatile bool flag1 = true;
volatile bool flag2 = false;

//--------------------------------          core 1          ---------------------------------

void setup() {
  setupOled();
}

void loop() {
  if (flag1) {
    flag2 = true;
    updateOled();
    flag2 - false;
  }
}

void setupOled() {
  oled.init();  // инициализация
}

void updateOled() {
  if (millis() - timer2 > displayRefreshDelay) {
    timer2 = millis(); // reset the timer
    oled.clear();
    oled.setScale(1);
    oled.home();  // home 0,0
    oled.print(gpsHour); oled.print(" ");
    oled.print(gpsMin); oled.print(" ");
    oled.print(gpsSec); oled.print(" ");
    oled.print(gpsDay); oled.print(" ");
    oled.print(gpsMon); oled.print(" ");
    oled.print(gpsYear); oled.println(" ");
    oled.print(gpsFix); oled.print(" ");
    oled.print(gpsQual); oled.println(" ");
    oled.print(gpsLatitude); oled.print(" ");
    oled.print(gpsLat); oled.println(" ");
    oled.print(gpsLongitude); oled.print(" ");
    oled.print(gpsLon); oled.println(" ");
    oled.print(gpsSpeed); oled.print(" ");
    oled.print(gpsAngle); oled.print(" ");
    oled.print(gpsAltitude); oled.println(" ");
    oled.print(gpsSat); oled.print(" ");
    oled.print(gpsAntenna); oled.print(" ");
    oled.print(imuTemp); oled.println(" ");
    oled.print(imuAccX); oled.print(" ");
    oled.print(imuAccY); oled.print(" ");
    oled.print(imuAccZ); oled.println(" ");
    oled.print(imuGyroX); oled.print(" ");
    oled.print(imuGyroY); oled.print(" ");
    oled.print(imuGyroZ); oled.print(" ");
    //oled.setCursor(5, 1);   // курсор в (пиксель X, строка Y)
    //oled.setScale(2);
    oled.update();
  }
}


//--------------------------------          core 1          ---------------------------------

void setup1() {
  setupGps();
}

void loop1() {
  if (flag2) {
    flag1 = true;
    updateGps();
    flag1 = false;
  }
}

void setupGps() {
  serialMonitor.begin(115200);
  serialMonitor.println("Adafruit GPS library basic parsing test!");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  serialGps.println(PMTK_Q_RELEASE);
}

void updateGps() {
  char c = GPS.read();
  if (GPSECHO)
    if (c) serialMonitor.print(c);
  if (GPS.newNMEAreceived()) {
    serialMonitor.print(GPS.lastNMEA());
    if (!GPS.parse(GPS.lastNMEA()))
      return;
  }
  if (millis() - timer1 > gpsRefreshDelay) {
    timer1 = millis(); // reset the timer
    gpsHour = GPS.hour;
    gpsMin = GPS.minute;
    gpsSec = GPS.seconds;
    gpsDay = GPS.day;
    gpsMon = GPS.month;
    gpsYear = GPS.year;
    gpsFix = GPS.fix;
    gpsQual = GPS.fixquality;
    gpsLatitude = GPS.latitude;
    gpsLat = GPS.lat;
    gpsLongitude = GPS.longitude;
    gpsLon = GPS.lon;
    gpsSpeed = GPS.speed;
    gpsAngle = GPS.angle;
    gpsAltitude = GPS.altitude;
    gpsSat = GPS.satellites;
    gpsAntenna = GPS.antenna;
    updateImu();
    serialMonitor.print("\nTime: ");
    if (GPS.hour < 10) {
      serialMonitor.print('0');
    }
    serialMonitor.print(GPS.hour, DEC); serialMonitor.print(':');
    if (GPS.minute < 10) {
      serialMonitor.print('0');
    }
    serialMonitor.print(GPS.minute, DEC); serialMonitor.print(':');
    if (GPS.seconds < 10) {
      serialMonitor.print('0');
    }
    serialMonitor.print(GPS.seconds, DEC); serialMonitor.print('.');
    if (GPS.milliseconds < 10) {
      serialMonitor.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      serialMonitor.print("0");
    }
    serialMonitor.println(GPS.milliseconds);
    serialMonitor.print("Date: ");
    serialMonitor.print(GPS.day, DEC); serialMonitor.print('/');
    serialMonitor.print(GPS.month, DEC); serialMonitor.print("/20");
    serialMonitor.println(GPS.year, DEC);
    serialMonitor.print("Fix: "); serialMonitor.print((int)GPS.fix);
    serialMonitor.print(" quality: "); serialMonitor.println((int)GPS.fixquality);
    if (GPS.fix) {
      serialMonitor.print("Location: ");
      serialMonitor.print(GPS.latitude, 4); serialMonitor.print(GPS.lat);
      serialMonitor.print(", ");
      serialMonitor.print(GPS.longitude, 4); serialMonitor.println(GPS.lon);
      serialMonitor.print("Speed (knots): "); serialMonitor.println(GPS.speed);
      serialMonitor.print("Angle: "); serialMonitor.println(GPS.angle);
      serialMonitor.print("Altitude: "); serialMonitor.println(GPS.altitude);
      serialMonitor.print("Satellites: "); serialMonitor.println((int)GPS.satellites);
      serialMonitor.print("Antenna status: "); serialMonitor.println((int)GPS.antenna);
    }
  }
}

void updateImu() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(imuAccX, imuAccY, imuAccZ);
  }
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(imuGyroX, imuGyroY, imuGyroZ);
  }
  if (IMU.temperatureAvailable()) {
    IMU.readTemperature(imuTemp);
  }
}
