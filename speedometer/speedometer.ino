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
  Formula:
  k = d × r × 0.001885

  Where:
  k = Kilometer Per Hour(km/hr)
  d = Wheel Diameter(cm)
  r = Revolution Per Minute(RPM)
*/

#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#define sensorPin 32

unsigned long TimerCount;
unsigned long previousTimerCount;

byte kilosPerHour = 0;
byte wheelDiamter = 28;
unsigned int revPerMinute = 0;
unsigned long finalSpeedKph = 0;
int pinState;

void setup() {
  delay(2000);
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  TimerCount = millis();
  previousTimerCount = millis();

}

void loop() {
  calculateRpm();
  calculateSpeed();
  drawDisplay();
  TimerCount = millis();
  //LCDprintValue = (((unsigned long)41887.8) / (TimerCount - previousTimerCount));
  previousTimerCount = TimerCount;
}

void calculateRpm() {
  pinState = digitalRead(sensorPin);
  if (pinState == HIGH) {
    Serial.println("HIGH");
  } else {
    Serial.println("LOW");
  }
}

void calculateSpeed() {
  finalSpeedKph = wheelDiamter * revPerMinute * 0.001885;
}

void drawDisplay() {
  //tft.fillRect(130, 180, 240, 250, 0x0000);
  tft.drawLine(0, 163, 320, 163, TFT_GREEN);
  tft.println(finalSpeedKph);
}
