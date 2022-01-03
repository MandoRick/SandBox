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

/*
  Formulae:
  r = m / t
  k = d × r × 0.001885

  Where:
  k = Kilometer Per Hour(km/hr)
  d = Wheel Diameter(cm)
  r = Revolution Per Minute(RPM)
  m = Millis 60000 per minute
  t = Time taken for rotation
*/

#define wheelDiam 28
#define sensorPin1 32
#define sensorPin2 35
#define tempSensor 36

#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

bool sensorState1 = true;
bool sensorState2 = true;
bool flipFlop = false;
bool udpateDisplay = true;

byte kilosPerHour = 0;
unsigned long revsPerMin = 0;
unsigned long finalSpeedKph = 0;
uint32_t rotationCount;

unsigned long currentMillis;
unsigned long previousMillis = 0;
unsigned long rotationTime;
int16_t currentTemp;

void setup() {
  delay(2000);
  Serial.begin(115200);
  pinMode(sensorPin1, INPUT_PULLUP);
  pinMode(sensorPin2, INPUT_PULLUP);
  setupDisplay();
  delay(1000);
  currentMillis = millis();
  previousMillis = millis();
}

void loop() {
  calculateSpeed();
  checkTemp();
  drawDisplay();
}

void setupDisplay() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  tft.drawLine(0, 200, 320, 200, TFT_GREEN);
  tft.setCursor(20, 25);
  tft.println("KPH");
  tft.setCursor(20, 100);
  tft.println("RPM");
  tft.setCursor(20, 150);
  tft.println("TEMP");
}

void calculateSpeed() {
  if (digitalRead(sensorPin1) == 0) {
    //Serial.println("Sensor 1 LOW");
    sensorState1 = false;
  }
  if (digitalRead(sensorPin2) == 0) {
    //Serial.println("Sensor 2 LOW");
    sensorState2 = false;
  }
  if (sensorState1 == false && sensorState2 == false) {
    sensorState1 = true;
    sensorState2 = true;
    flipFlop = !flipFlop;
    if (flipFlop == true) {
      currentMillis = millis();
      rotationTime = currentMillis - previousMillis;
      revsPerMin = 60000 / rotationTime;
      previousMillis = currentMillis;
      rotationCount += 1;
      finalSpeedKph = wheelDiam * revsPerMin * 0.001885;
      udpateDisplay = true;
      Serial.printf("Sensor triggered %u times\n", rotationCount);
      Serial.printf("Rotation time %u millis\n", rotationTime);
      Serial.printf("Rotation %u RPM\n", revsPerMin);
      Serial.printf("Speed %u KPH\n", finalSpeedKph);
    }
  }
}

void checkTemp() {
  uint16_t tempValue = analogRead(tempSensor);
  float pinVoltage = (tempValue / 4096) * 5;
  float tempCelcius = (pinVoltage - 0.5) * 100;
  currentTemp = tempCelcius;
  //Serial.println("temp C: " +(String)tempCelcius);
}

void drawDisplay() {
  if (udpateDisplay == true) {
    tft.setTextSize(7);
    tft.fillRect(100, 0, 200, 170, TFT_BLACK);
    tft.setTextColor(TFT_GREEN);
    tft.setCursor(100, 25);
    tft.println(finalSpeedKph);
    tft.setTextSize(4);
    tft.setCursor(100, 100);
    tft.println(revsPerMin);
    tft.setTextSize(3);
    tft.setCursor(100, 150);
    tft.print(currentTemp);
    tft.println("`C");
    udpateDisplay = false;
  }
}
