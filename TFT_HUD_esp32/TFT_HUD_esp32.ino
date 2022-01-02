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

#include <SPI.h>
#include <TFT_eSPI.h>
#include <ESP32Servo.h>
#define servoPin1 2
#define echoPin 33
#define trigPin 32

Servo myservo1;
TFT_eSPI tft = TFT_eSPI();

#define debug 1     // 1 ON 0 OFF
#if debug == 1
#define debugln(x) Serial.println(x)
#define debug(x) Serial.print(x)
#else
#define debugln(x)
#define debug(x)
#endif

#define ledPin  LED_BUILTIN
#define displayWidth 320
#define displayHeight 240

uint16_t servoDelay = 5;
long duration; // variable for the duration of sound wave travel
int16_t distance_cm; // variable for centimeters measurement
int16_t servoIncrement = 1;
int16_t pos1 = 0;
uint16_t sweepMapX;
uint16_t sweepMapOldX;
uint16_t dotDistance;
uint16_t dotDistanceOld;

//------- version stuff --------
const String p_project = "default";
const uint8_t version_hi = 0;
const uint8_t version_lo = 1;
void versionPrint(void) {
  debug("RicksWorx: ");
  debugln(p_project);
  debug("Version: ");
  debug(version_hi);
  debug(".");
  debugln(version_lo);
}
//------- end version stuff --------



void setup() {
  delay(2000);
  Serial.begin(115200);
  debugln("Serial started");
  versionPrint();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setTextSize(2);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo1.setPeriodHertz(240 );    // standard 50 hz servo
  myservo1.attach(servoPin1, 0, 2400);
}


void loop() {
  moveServo();
}

void moveServo() {
  myservo1.write(pos1);
  delay(servoDelay);
  pos1 = pos1 + servoIncrement;
  if (pos1 <= 0 || pos1 >= 180) {
    servoIncrement = -servoIncrement;
  }
  pingSonic();
  drawDisplay();
}

void pingSonic() {
  sweepMapOldX = sweepMapX;
  dotDistanceOld = dotDistance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance_cm = duration * 0.034 / 2;
  //debugln("Distance: " + (String)distance_cm);
}


void drawDisplay() {
  sweepMapX = map(pos1, 0, 180, 0, 320);
  dotDistance = map(distance_cm, 1, 1197, 160, 0);
  tft.fillRect(130, 180, 240, 250, 0x0000);
  tft.drawLine(0, 163, 320, 163, TFT_GREEN);
  tft.drawLine(sweepMapX - 2, 160, sweepMapX - 2, 0, TFT_BLACK);
  tft.drawLine(sweepMapX + 2, 160, sweepMapX + 2, 0, TFT_BLACK);
  tft.drawLine(sweepMapX - 3, 160, sweepMapX - 3, 0, TFT_BLACK);
  tft.drawLine(sweepMapX + 3, 160, sweepMapX + 3, 0, TFT_BLACK);
  tft.drawCircle(sweepMapX, dotDistance , 2, TFT_GREEN);
  tft.drawLine(sweepMapX, 160, sweepMapX, 0, TFT_GREEN);
  tft.drawLine(sweepMapX - 1, 160, sweepMapX - 1, 0, TFT_DARKGREEN);
  tft.drawLine(sweepMapX + 1, 160, sweepMapX + 1, 0, TFT_DARKGREEN);
  tft.drawCircle(sweepMapOldX, dotDistanceOld, 2, TFT_RED);
  tft.setCursor(140, 200);
  tft.println(distance_cm);
}
