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
#include <Pixy2I2C.h> //cam
#include <Servo.h> //servo
#include "notes.h"

#define SERIAL_GPS Serial1
#define SERIAL_MONITOR Serial
#define GPSECHO (false)
#define GPS_REFRESH_DELAY (2000)
#define PIN_TRIG (28)
#define PIN_ECHO (29)
#define LED_PIN (LED_BUILTIN)
#define BUZZER_PIN (5)
#define SERVO_PIN (21)
#define SERVO_ANGLE (130)
#define SERVO_SPEED (25)
#define RANDOM_SEED_PIN (13)
int servo_pos = 0;
int temperature_deg;
int16_t gpsHour, gpsMin, gpsSec, gpsDay, gpsMon, gpsYear, gpsFix, gpsQual, gpsSat , gpsAntenna, gpsLat, gpsLon, gpsSpeed, gpsAltitude, gpsAngle;
float gpsLatitude, gpsLongitude, ax, ay, az, gx, gy, gz;
uint32_t timer1 = millis();
uint32_t timer2 = millis();

Adafruit_GPS GPS(&SERIAL_GPS);
GyverOLED<SSH1106_128x64> OLED;
Pixy2I2C PIXY;
Servo CAM_SERVO;

// -------------------------------  core 0 -------------------------------

void setup() {
  SERIAL_MONITOR.begin(115200);
  delay(2500);
  PIXY.init();
  SERIAL_MONITOR.println(PIXY.changeProg("line"));
  delay(2500);
  OLED.init();
  delay(2500);
  SERIAL_MONITOR.println("Ricks Drone!");
  //while (!SERIAL_MONITOR);
  if (!IMU.begin()) {
    SERIAL_MONITOR.println("Failed to initialize IMU!");
    while (1);
  }
  delay(2500);
  setupGps();
  delay(2500);
}

void loop() {
  updateImu();
  updateGps();
  //updatePixy();
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
    if (c) SERIAL_MONITOR.print(c);
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trying to print out data
    SERIAL_MONITOR.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
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
    imuSerialPrint();
  }
}

void imuSerialPrint() {
  SERIAL_MONITOR.println(" ");
  SERIAL_MONITOR.print(gpsHour); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsMin); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsSec); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsDay); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsMon); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsYear); SERIAL_MONITOR.println(" ");
  SERIAL_MONITOR.print(gpsFix); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsQual); SERIAL_MONITOR.println(" ");
  SERIAL_MONITOR.print(gpsLatitude); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsLat); SERIAL_MONITOR.println(" ");
  SERIAL_MONITOR.print(gpsLongitude); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsLon); SERIAL_MONITOR.println(" ");
  SERIAL_MONITOR.print(gpsSpeed); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsAngle); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsAltitude); SERIAL_MONITOR.println(" ");
  SERIAL_MONITOR.print(gpsSat); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gpsAntenna); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(temperature_deg); SERIAL_MONITOR.println(" ");
  SERIAL_MONITOR.print(ax); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(ay); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(az); SERIAL_MONITOR.println(" ");
  SERIAL_MONITOR.print(gx); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gy); SERIAL_MONITOR.print(" ");
  SERIAL_MONITOR.print(gz); SERIAL_MONITOR.println(" ");
}

void updateOled() {
  OLED.clear();
  OLED.setScale(1);
  OLED.home();  // home 0,0
  OLED.print(gpsHour); OLED.print(" ");
  OLED.print(gpsMin); OLED.print(" ");
  OLED.print(gpsSec); OLED.print(" ");
  OLED.print(gpsDay); OLED.print(" ");
  OLED.print(gpsMon); OLED.print(" ");
  OLED.print(gpsYear); OLED.println(" ");
  OLED.print(gpsFix); OLED.print(" ");
  OLED.print(gpsQual); OLED.println(" ");
  OLED.print(gpsLatitude); OLED.print(" ");
  OLED.print(gpsLat); OLED.println(" ");
  OLED.print(gpsLongitude); OLED.print(" ");
  OLED.print(gpsLon); OLED.println(" ");
  OLED.print(gpsSpeed); OLED.print(" ");
  OLED.print(gpsAngle); OLED.print(" ");
  OLED.print(gpsAltitude); OLED.println(" ");
  OLED.print(gpsSat); OLED.print(" ");
  OLED.print(gpsAntenna); OLED.print(" ");
  OLED.print(temperature_deg); OLED.println(" ");
  OLED.print(ax); OLED.print(" ");
  OLED.print(ay); OLED.print(" ");
  OLED.print(az); OLED.println(" ");
  OLED.print(gx); OLED.print(" ");
  OLED.print(gy); OLED.print(" ");
  OLED.print(gz); OLED.print(" ");
  //OLED.setCursor(5, 1);   // курсор в (пиксель X, строка Y)
  //OLED.setScale(2);
  OLED.update();
}

void updatePixy() {
  if (millis() - timer2 > 250) {
    timer2 = millis(); // reset the timer
    int8_t i;
    char buf[128];
    PIXY.line.getMainFeatures();
    if (PIXY.line.numVectors)
      PIXY.line.vectors->print();
    if (PIXY.line.numIntersections)
      PIXY.line.intersections->print();
    if (PIXY.line.barcodes)
      PIXY.line.barcodes->print();
  }
}


// -------------------------------  core 1 -------------------------------

//------- tone generation --------
void playNote(int frequency, int duration, bool hold = false, bool measure = true) {
  (void) measure;
  if (hold) {
    // For a note that's held play it a little longer than the specified duration
    // so it blends into the next tone (but there's still a small delay to
    // hear the next note).
    tone(BUZZER_PIN, frequency, duration + duration / 32);
  } else {
    // For a note that isn't held just play it for the specified duration.
    tone(BUZZER_PIN, frequency, duration);
  }
  delay(duration + duration / 16);
}
//--------------------------

void setup1() {
  delay(2500);
  setupUltrasonic();
  delay(2500);
  pinMode(LED_PIN, OUTPUT);
  delay(2500);
  CAM_SERVO.attach(SERVO_PIN);
  randomSeed(analogRead(RANDOM_SEED_PIN));
}

void loop1() {
  updateUltrasonic();
  delay(10000);
  droneSpeak();
  delay(10000);
  moveServo();
  delay(10000);
  droneSpeak();
  delay(10000);
}

void setupUltrasonic() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
}

void updateUltrasonic() {
  // Start a new measurement:
  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  // Read the result:
  int duration = pulseIn(PIN_ECHO, HIGH);
  int ledBrightness = map(duration, 0, 1000, 0, 255);
  analogWrite(LED_PIN, ledBrightness);
  //Serial.print("Distance in CM: ");
  //Serial.println(duration / 58);
  //Serial.print("Distance in inches: ");
  //Serial.println(duration / 148);
}

void moveServo() {
  for (servo_pos = 0; servo_pos <= SERVO_ANGLE; servo_pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    CAM_SERVO.write(servo_pos);              // tell servo to go to position in variable 'pos'
    delay(SERVO_SPEED);                       // waits 15ms for the servo to reach the position
  }
  delay(SERVO_SPEED);
  for (servo_pos = SERVO_ANGLE; servo_pos >= 0; servo_pos -= 1) { // goes from 180 degrees to 0 degrees
    CAM_SERVO.write(servo_pos);              // tell servo to go to position in variable 'pos'
    delay(SERVO_SPEED);                       // waits 15ms for the servo to reach the position
  }
}

void droneSpeak() {
  playNote(random(100, 4000), SIXTEENTH, false);
  playNote(random(100, 4000), SIXTEENTH, false);
  playNote(random(100, 4000), SIXTEENTH, false);
  playNote(random(100, 4000), SIXTEENTH, false);
  playNote(random(100, 4000), SIXTEENTH, false);
}
