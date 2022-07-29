
#include <Adafruit_GPS.h>

#define serialGps Serial1
#define serialMonitor Serial
#define flagPin1 20

Adafruit_GPS GPS(&serialGps);

#define GPSECHO true

int32_t gpsHour, gpsMin, gpsSec, gpsDay, gpsMon, gpsYear, gpsFix, gpsQual, gpsLatitude, gpsLat, gpsLongitude, gpsLon, gpsSpeed, gpsAngle, gpsAltitude, gpsSat , gpsAntenna;

uint32_t timer = millis();

void setup() {
  pinMode(flagPin1, INPUT);
}

void loop() {
  if (digitalRead(flagPin1) == LOW) {

  }
}

void setup1() {
  serialMonitor.begin(115200);
  serialMonitor.println("Adafruit GPS library basic parsing test!");
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
  GPS.sendCommand(PGCMD_ANTENNA);
  delay(1000);
  serialGps.println(PMTK_Q_RELEASE);
  pinMode(flagPin1, OUTPUT);
}

void loop1() {
  char c = GPS.read();
  if (GPSECHO)
    if (c) serialMonitor.print(c);
  if (GPS.newNMEAreceived()) {
    serialMonitor.print(GPS.lastNMEA());
    if (!GPS.parse(GPS.lastNMEA()))
      return;
  }
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
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer
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
