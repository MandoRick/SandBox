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

#include "defines.h"
#include "login.h"
#define WIFI_FIRMWARE_LATEST_VERSION "1.4.8"
#include <SPI.h>
#include <WiFiNINA_Generic.h>
#include <Wire.h>  //i2c
#include <GyverOLED.h>  //display
#include <BH1750_WE.h> //light sensor
#include <MQ135.h>  //air quality sensor
#include <microDS18B20.h> //ext temp sensor
#include <RTClib.h> //real time clock
#include <GyverBME280.h> //temp press hum sensor
#include <Arduino_LSM6DSOX.h>  //6dof sensor

#define SEALEVELPRESSURE_HPA (1013.25)
#define BH1750_ADDRESS (0x23)
#define WIND_SPEED_SENSOR_PIN_1 (21)  //white
#define WIND_SPEED_SENSOR_PIN_2 (20)  //green
#define RELAY_SIGNAL_0 (19) //white
#define RELAY_SIGNAL_1 (18) //green
#define WIND_DIRECTION_SENSOR_PIN_1 (A0)
#define PIN_MQ135 (A2)
#define BUILTIN_LED_PIN (LED_BUILTIN)
#define TEMP_PROBE_SENSOR_PIN (5)

GyverBME280 bme;
BH1750_WE myBH1750(BH1750_ADDRESS);
GyverOLED<SSH1106_128x64> oled;
MQ135 mq135_sensor(PIN_MQ135);
MicroDS18B20<TEMP_PROBE_SENSOR_PIN> extTempProbe;
RTC_DS3231 rtc;
WiFiServer server(80);

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP), length must be 8+
int keyIndex = 0;                 // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
volatile long windDirectionSensorValue = 0;  // variable to store the value coming from the speed sensor
volatile long oldWindDirectionSensorValue = 0;
volatile long windSpeedRotationCount = 0;  // variable to store the value coming from the direction sensor
volatile unsigned long windSpeedRotations = 0;
volatile unsigned long oldWindSpeedRotations = 0;
volatile unsigned long currentMicros = 0;
volatile unsigned long previousMicros = 0;
volatile unsigned long timeSinceLastRotation = 0;
float currentWindSpeed = 0;
uint16_t currentWindDirection = 0;
bool windDirectionClockwise = true;
bool autoLights = true;
bool lightsActivated = false;
float atmoPressure = 0;
float atmoMoisture = 0;
float intTemp = 0;
float atmoAltitude = 0;
float currentLightLevel = 0;
float oldLightLevel = 0;
float windSpeed = 0;
float windHeading = 0;
float rainQty = 0;
float correctedPPM = 0;
float extTemp = 0;
float imuAccX, imuAccY, imuAccZ;
float imuGyroX, imuGyroY, imuGyroZ;
float humidity = 25.0;
int boardTemp;
int currentHour, currentMin, currentSec, currentDay, currentMon, currentYear;
String currentWeekday;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
bool rainbowActive = false;
word led1Brightness = 0;
word fadeAmount1 = 1;
word led2Brightness = 125;
word fadeAmount2 = 1;
word led3Brightness = 254;
word fadeAmount3 = 1;

void wifiSetup() {
  while (!Serial && millis() < 5000);
  Serial.print(F("\nStart SimpleWebServerWiFi on ")); Serial.println(BOARD_NAME);
  Serial.println(WIFININA_GENERIC_VERSION);
  if (WiFi.status() == WL_NO_MODULE) {       // check for the WiFi module:
    Serial.println(F("Communication with WiFi module failed!"));
    while (true); // don't continue
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.print(F("Your current firmware NINA FW v"));
    Serial.println(fv);
    Serial.print(F("Please upgrade the firmware to NINA FW v"));
    Serial.println(WIFI_FIRMWARE_LATEST_VERSION);
  }
  while (status != WL_CONNECTED) {  // attempt to connect to WiFi network:
    Serial.print(F("Attempting to connect to Network named: "));
    Serial.println(ssid);                   // print the network name (SSID);
    status = WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWiFiStatus();                        // you're connected now, so print out the status
}

void setupOled() {
  oled.init();
  oled.clear();
  oled.update();
}

void setup() {
  Serial.begin(115200);
  delay(50);
  wifiSetup();
  delay(50);
  Wire.begin();
  delay(50);
  bme.begin(0x76);
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  delay(50);
  myBH1750.init();
  delay(50);
  rtc.begin();
  delay(50);
  IMU.begin();
  delay(50);
  setupOled();
  delay(50);
  pinMode(WIND_SPEED_SENSOR_PIN_1, INPUT_PULLUP);
  pinMode(WIND_SPEED_SENSOR_PIN_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WIND_SPEED_SENSOR_PIN_1), sensorPinInterrupt1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(WIND_SPEED_SENSOR_PIN_2), sensorPinInterrupt2, CHANGE);
  pinMode(RELAY_SIGNAL_0, OUTPUT);
  pinMode(RELAY_SIGNAL_1, OUTPUT);
}

void loop() {
  checkSensors();
  calculateResults();
  checkWifi();
  displayResultsOled();
}

void checkSensors() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(imuAccX, imuAccY, imuAccZ);
  }
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(imuGyroX, imuGyroY, imuGyroZ);
  }
  if (IMU.temperatureAvailable()) {
    IMU.readTemperature(boardTemp);
  }
  windDirectionSensorValue = analogRead(WIND_DIRECTION_SENSOR_PIN_1);
  delay(25);
  extTempProbe.requestTemp();
  delay(25);
  extTemp = extTempProbe.getTemp();
  delay(25);
  intTemp = bme.readTemperature();
  delay(25);
  atmoMoisture = bme.readHumidity();
  delay(25);
  atmoPressure = bme.readPressure() / 100.0F;
  delay(25);
  atmoAltitude = atmoPressure * SEALEVELPRESSURE_HPA;
  delay(25);
  currentLightLevel = myBH1750.getLux();
  delay(25);
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(intTemp, atmoMoisture);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  correctedPPM = mq135_sensor.getCorrectedPPM(intTemp, atmoMoisture);
  DateTime now = rtc.now();
  currentHour = now.hour();
  currentMin = now.minute();
  currentSec = now.second();
  currentDay = now.day();
  currentMon = now.month();
  currentYear = now.year();
  currentWeekday = (daysOfTheWeek[now.dayOfTheWeek()]);
  delay(25);
}

void calculateResults() {
  if (windSpeedRotations != oldWindSpeedRotations) {
    oldWindSpeedRotations = windSpeedRotations;
    currentMicros = millis();
    timeSinceLastRotation = currentMicros - previousMicros;
    previousMicros = currentMicros;
    if (timeSinceLastRotation >= 2500) {
      timeSinceLastRotation = 2500;
    }
    currentWindSpeed = map(timeSinceLastRotation, 2500, 250, 0, 100);
  }
  if (autoLights) {
    if (currentLightLevel != oldLightLevel) {
      oldLightLevel = currentLightLevel;
      if (currentLightLevel >= 0.25) {
        lightsOff();
        lightsActivated = false;
      }
      else {
        lightsOn();
        lightsActivated = true;
      }
    }
  }
  if (rainbowActive) {
    rainbowFade();
  }
  if (windDirectionSensorValue != oldWindDirectionSensorValue) {
    oldWindDirectionSensorValue = windDirectionSensorValue;
    if (windDirectionSensorValue > oldWindDirectionSensorValue) {
      windDirectionClockwise = true;
    } else {
      windDirectionClockwise = false;
    }
    if (windDirectionClockwise) {
      currentWindDirection = map(windDirectionSensorValue, 590, 800, 0, 359);
    } else {
      currentWindDirection = map(windDirectionSensorValue, 590, 800, 359, 0);
    }
  }
}

void checkWifi() {
  WiFiClient client = server.available();   // listen for incoming clients
  if (client) {       // if you get a client
    Serial.println(F("New client"));           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {       // loop while the client's connected
      if (client.available())  {       // if there's bytes to read from the client
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {     // if the byte is a newline character if the current line is blank, you got two newline characters in a row.
          if (currentLine.length() == 0) {      // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            client.println("HTTP/1.1 200 OK");   // and a content-type so the client knows what's coming, then a blank line:
            client.println("Content-type:text/html");
            client.println();
            client.print("<!DOCTYPE HTML>"
                         "<html>"
                         "<body style=\"background-color:rgb(18, 18, 18);\">"
                         "<h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Ext Temp: </span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(extTemp);
            client.print(" c"
                         "</h3></span>"
                         "<h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Int Temp: </span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(intTemp);
            client.print(" c"
                         "</h3></span>"
                         "<h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Board Temp: </span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(boardTemp);
            client.print(" c"
                         "</h3></span>"
                         "<h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Atmo Press: </span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(atmoPressure);
            client.print(" hpa"
                         "</h3></span>"
                         "<h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Humidity: </span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(atmoMoisture);
            client.print(" %</h3></span>"
                         "<h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Air Quality: </span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(correctedPPM);
            client.print(" ppm"
                         "</h3></span>"
                         "<h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Acc: X</span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(imuAccX);
            client.print("</span>"
                         "<span style=\"font-family:courier;color:rgb(0, 102, 255);\"> Y</span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(imuAccY);
            client.print("</span>"
                         "<span style=\"font-family:courier;color:rgb(0, 102, 255);\"> Z</span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(imuAccZ);
            client.print("</h3></span>"
                         "<h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Gyro: X</span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(imuGyroX);
            client.print("</span>"
                         "<span style=\"font-family:courier;color:rgb(0, 102, 255);\"> Y</span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(imuGyroY);
            client.print("</span>"
                         "<span style=\"font-family:courier;color:rgb(0, 102, 255);\"> Z</span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(imuGyroZ);
            client.print("</h3></span>"
                         "<h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Light Level: </span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(currentLightLevel);
            client.print("%</h3></span><h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Time: </span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(currentHour); client.print(":"); client.print(currentMin); client.print(":"); client.print(currentSec); client.print("<br>");
            client.print(currentDay); client.print("-"); client.print(currentMon); client.print("-"); client.print(currentYear);
            client.print("</h3></span><h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Wind Speed: </span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(currentWindSpeed);
            client.print(" mps</h3></span><h3 span style=\"font-family:courier;color:rgb(0, 102, 255);\">Wind Dir: </span>"
                         "<span style=\"font-family:courier;color:rgb(255, 0, 212);\">");
            client.print(currentWindDirection);
            client.print("</h3></span><br />"
                         "<style>"
                         ".container {margin: 0 auto; text-align: center; margin-top: 5px;}"
                         "button {color: rgb(0, 102, 255); width: 80px; height: 30px;"
                         "border-radius: 5%; margin: 5px; border: none; font-size: 18px; outline: none; transition: all 0.2s;}"
                         ".dark{background-color: rgb(28, 28, 28);}"
                         "button:hover{cursor: pointer; opacity: 0.7;}"
                         "</style>"
                         "<div class='container'>"
                         "<button class='dark' type='submit' onmousedown='location.href=\"ON\"'>ON</button>  "
                         "<button class='dark' type='submit' onmousedown='location.href=\"OF\"'>OFF</button><br>"
                         "<button class='dark' type='submit' onmousedown='location.href=\"AU\"'>AUTO</button><br>"
                         "</div>"
                         "</html>");
            client.println();           // The HTTP response ends with another blank line:
            break;    // break out of the while loop:
          }
          else  {     // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {     // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
        if (currentLine.endsWith("GET /ON")) {
          autoLights = false;
          lightsActivated = true;
          lightsOn();
        }
        if (currentLine.endsWith("GET /OF")) {
          autoLights = false;
          lightsActivated = false;
          lightsOff();
        }
        if (currentLine.endsWith("GET /AU")) {
          autoLights = true;
        }
      }
    }
    client.stop();    // close the connection:
    Serial.println(F("Client disonnected"));
  }
}

void displayResultsOled() {
  oled.clear();
  oled.setScale(1);
  oled.setCursor(0, 0);
  oled.println("Spd:" + (String)currentWindSpeed + "  Dir:" + (String)currentWindDirection);
  oled.setCursor(0, 1);
  oled.println("E:" + (String)extTemp + "I:" + (String)intTemp + " B:" + (String)boardTemp + " C");
  oled.setCursor(0, 2);
  oled.println("AQ:" + (String)correctedPPM + " LL:" + (String)currentLightLevel);
  oled.setCursor(0, 3);
  oled.println("H:" + (String)atmoMoisture + " P:" + (String)atmoPressure + " A:" + (String)atmoAltitude);
  oled.setCursor(0, 4);
  oled.println("AccX:" + (String)imuAccX + " Y:" + (String)imuAccY + " Z:" + (String)imuAccZ);
  oled.setCursor(0, 5);
  oled.println("GyroX:" + (String)imuGyroX + " Y:" + (String)imuGyroY + " Z:" + (String)imuGyroZ);
  oled.setCursor(0, 6);
  oled.println((String)currentDay + "/" + (String)currentMon + "/" + (String)currentYear + " " + (String)currentWeekday);
  oled.setCursor(0, 7);
  oled.println((String)currentHour + ":" + (String)currentMin + ":" + (String)currentSec);
  if (autoLights) {
    oled.setCursor(65, 7);
    oled.print("AutLi: ");
    if (lightsActivated) {
      oled.print("ON");
    }
    else {
      oled.print("OFF");
    }
  }
  oled.update();
  delay(50);
}

void printWiFiStatus() {
  Serial.print(F("SSID: "));// print the SSID of the network you're attached to:
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();// print your board's IP address:
  Serial.print(F("IP Address: "));
  Serial.println(ip);
  long rssi = WiFi.RSSI();  // print the received signal strength:
  Serial.print(F("Signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
  Serial.print(F("To see this page in action, open a browser to http://"));  // print where to go in a browser:
  Serial.println(ip);
}

void lightsOff() {
  digitalWrite(RELAY_SIGNAL_0, HIGH);
  analogWrite(LEDR, 255);
  analogWrite(LEDG, 255);
  analogWrite(LEDB, 255);
}

void lightsOn() {
  digitalWrite(RELAY_SIGNAL_0, LOW);
  analogWrite(LEDR, 100);
  analogWrite(LEDG, 100);
  analogWrite(LEDB, 100);
}

void rainbowFade() {
  analogWrite(LEDR, led1Brightness);
  analogWrite(LEDG, led2Brightness);
  analogWrite(LEDB, led3Brightness);
  led1Brightness = led1Brightness + fadeAmount1;
  led2Brightness = led2Brightness + fadeAmount2;
  led3Brightness = led3Brightness + fadeAmount3;
  if (led1Brightness <= 0 || led1Brightness >= 255) {
    fadeAmount1 = -fadeAmount1;
  }
  if (led2Brightness <= 0 || led2Brightness >= 255) {
    fadeAmount2 = -fadeAmount2;
  }
  if (led3Brightness <= 0 || led3Brightness >= 255) {
    fadeAmount3 = -fadeAmount3;
  }
  if (led1Brightness == 255) {
    led1Brightness = random(0, 255);
  }
  if (led2Brightness == 255) {
    led2Brightness = random(0, 255);
  }
  if (led3Brightness == 255) {
    led3Brightness = random(0, 255);
  }
}

void windSpeedtick() {
  windSpeedRotationCount++;
  if (windSpeedRotationCount >= 1200) {
    windSpeedRotations++;
    windSpeedRotationCount = 0;
  }
}

void sensorPinInterrupt1() {
  //windSpeedRotationCount++;
  //windSpeedtick();
}

void sensorPinInterrupt2() {
  windSpeedtick();
}
