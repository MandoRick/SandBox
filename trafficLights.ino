/*
  --------------- by JediRick -----------------
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
*/

#include <Arduino.h>
#include "tf.h"

#define rightGreen 2
#define rightYellow 4
#define rightRed 7
#define leftGreen 8
#define leftYellow 12
#define leftRed 13

TrafficLights tf1(rightRed, rightYellow, rightGreen, TrafficLights::States::RED);
TrafficLights tf2(leftRed, leftYellow, leftGreen, TrafficLights::States::GREEN);

void setup()
{
  Serial.begin(9600);
}

uint32_t currentTime;
uint32_t prevTime;
uint32_t waitDelay = 1000;

void loop()
{

  currentTime = millis();

  tf1.run();
  tf2.run();

  if (currentTime - prevTime > waitDelay)
  {
    prevTime = currentTime;
    tf1.nextState();
    tf2.nextState();
  }
}
