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
#include <Arduino_LSM6DSOX.h> //imu
#include <Adafruit_GPS.h> //gps

#define serialGps Serial1
#define serialMonitor Serial
#define GPSECHO (false)
#define gpsRefreshDelay (2000)

int temperature_deg;
int16_t gpsHour, gpsMin, gpsSec, gpsDay, gpsMon, gpsYear, gpsFix, gpsQual, gpsSat , gpsAntenna, gpsLat, gpsLon, gpsSpeed, gpsAltitude, gpsAngle;
float gpsLatitude, gpsLongitude, ax, ay, az, gx, gy, gz;
uint32_t timer1 = millis();

Adafruit_GPS GPS(&serialGps);
GyverOLED<SSH1106_128x64> oled;

void setup() {
  serialMonitor.begin(115200);
  delay(2000);
  oled.init();  // инициализация
  serialMonitor.println("Ricks Drone!");
  //while (!serialMonitor);
  if (!IMU.begin()) {
    serialMonitor.println("Failed to initialize IMU!");
    while (1);
  }
  delay(500);
  
  setupGps();
}

void loop() {
  updateImu();
  updateGps();
}

void setupGps() {
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ); // 1 Hz update rate
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz

  // Request updates on antenna status, comment out to keep quiet
  //GPS.sendCommand(PGCMD_ANTENNA);

  delay(1000);

  // Ask for firmware version
  //GPSSerial.println(PMTK_Q_RELEASE);
}

void updateImu() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);
  }
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);
  }
  if (IMU.temperatureAvailable()) {
    IMU.readTemperature(temperature_deg);
  }
}

void updateGps() {
  // read data from the GPS in the 'main loop'
  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if (GPSECHO)
    if (c) Serial.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer1 > 2000) {
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
    updateOled();
    //imuSerialPrint();
  }
}

void imuSerialPrint() {
  serialMonitor.println(" ");
  serialMonitor.print(gpsHour); serialMonitor.print(" ");
  serialMonitor.print(gpsMin); serialMonitor.print(" ");
  serialMonitor.print(gpsSec); serialMonitor.print(" ");
  serialMonitor.print(gpsDay); serialMonitor.print(" ");
  serialMonitor.print(gpsMon); serialMonitor.print(" ");
  serialMonitor.print(gpsYear); serialMonitor.println(" ");
  serialMonitor.print(gpsFix); serialMonitor.print(" ");
  serialMonitor.print(gpsQual); serialMonitor.println(" ");
  serialMonitor.print(gpsLatitude); serialMonitor.print(" ");
  serialMonitor.print(gpsLat); serialMonitor.println(" ");
  serialMonitor.print(gpsLongitude); serialMonitor.print(" ");
  serialMonitor.print(gpsLon); serialMonitor.println(" ");
  serialMonitor.print(gpsSpeed); serialMonitor.print(" ");
  serialMonitor.print(gpsAngle); serialMonitor.print(" ");
  serialMonitor.print(gpsAltitude); serialMonitor.println(" ");
  serialMonitor.print(gpsSat); serialMonitor.print(" ");
  serialMonitor.print(gpsAntenna); serialMonitor.print(" ");
  serialMonitor.print(temperature_deg); serialMonitor.println(" ");
  serialMonitor.print(ax); serialMonitor.print(" ");
  serialMonitor.print(ay); serialMonitor.print(" ");
  serialMonitor.print(az); serialMonitor.println(" ");
  serialMonitor.print(gx); serialMonitor.print(" ");
  serialMonitor.print(gy); serialMonitor.print(" ");
  serialMonitor.print(gz); serialMonitor.println(" ");
}

void updateOled() {
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
  oled.print(temperature_deg); oled.println(" ");
  oled.print(ax); oled.print(" ");
  oled.print(ay); oled.print(" ");
  oled.print(az); oled.println(" ");
  oled.print(gx); oled.print(" ");
  oled.print(gy); oled.print(" ");
  oled.print(gz); oled.print(" ");
  //oled.setCursor(5, 1);   // курсор в (пиксель X, строка Y)
  //oled.setScale(2);
  oled.update();
}
