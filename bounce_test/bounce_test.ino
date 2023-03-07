/*-data-type------size---------description----------------------
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
  --------------------------------------------------------------
  camelCase                -  anything that changes
  snake_case               -  variable's that are exclusive in a function
  Snake_Case               -  CLASS/struct exclusave varables/functions
  iNVERTEDcAMELcASE        -  outside code that is being accessed [database]
  SNake_CAse               -  duplicate varables inside the case function [frequently used in library names]
  ALL_CAPS                 -  const varable names or defines
  ------------- by jediRick & RefreshMyMind --------------------
*/

/*  Collision Detection Library
    by: JediRick and RefreshMyMind
    wokwi example link: https://wokwi.com/projects/356486662567493633
*/

#include <GyverOLED.h>
#include <CollisionCheck.h>

//you can collision check multiple objects for example:
//CollisionCheck colCheck1, colCheck2;
//for this demo we will do one collision check
CollisionCheck col1Check, col2Check, col3Check, col4Check, col5Check, col6Check, col7Check, col8Check, col9Check;


uint8_t ball1[]{ 96, 32, 6, 6 };
uint8_t ball2[]{ 32, 32, 2, 2 };
uint8_t object1[]{ 0, 0, 1, 64 };
uint8_t object2[]{ 126, 0, 1, 64 };
uint8_t object3[]{ 0, 0, 128, 1 };
uint8_t object4[]{ 0, 63, 128, 1 };

bool collis01Result = false;
bool collis02Result = false;
bool collis03Result = false;
bool collis04Result = false;
bool collis05Result = false;
bool collis06Result = false;
bool collis07Result = false;
bool collis08Result = false;
bool collis09Result = false;

int incrementX1 = 1;
int incrementY1 = 1;
int incrementX2 = -1;
int incrementY2 = -1;

GyverOLED<SSD1306_128x64, OLED_BUFFER> oled;

void setup() {
  Serial.begin(115200);
  oled.init();
  Wire.setClock(3200000);
  oled.clear();
  oled.update();
}

void loop() {
  collisionChecks();
  moveBall();
  drawBall();
  //delay(5);
}

void collisionChecks() {
  //collisionDetection(obj1X, obj1Y, obj1W, obj1H, obj2X, obj2Y, obj2W, obj2H);
  collis01Result = col1Check.collisionCheck(ball1[0], ball1[1], ball1[2], ball1[3], object1[0], object1[1], object1[2], object1[3]);
  collis02Result = col2Check.collisionCheck(ball1[0], ball1[1], ball1[2], ball1[3], object2[0], object2[1], object2[2], object2[3]);
  collis03Result = col3Check.collisionCheck(ball1[0], ball1[1], ball1[2], ball1[3], object3[0], object3[1], object3[2], object3[3]);
  collis04Result = col4Check.collisionCheck(ball1[0], ball1[1], ball1[2], ball1[3], object4[0], object4[1], object4[2], object4[3]);
  collis05Result = col5Check.collisionCheck(ball2[0], ball2[1], ball2[2], ball2[3], object1[0], object1[1], object1[2], object1[3]);
  collis06Result = col6Check.collisionCheck(ball2[0], ball2[1], ball2[2], ball2[3], object2[0], object2[1], object2[2], object2[3]);
  collis07Result = col7Check.collisionCheck(ball2[0], ball2[1], ball2[2], ball2[3], object3[0], object3[1], object3[2], object3[3]);
  collis08Result = col8Check.collisionCheck(ball2[0], ball2[1], ball2[2], ball2[3], object4[0], object4[1], object4[2], object4[3]);
  collis09Result = col9Check.collisionCheck(ball1[0], ball1[1], ball1[2], ball1[3], ball2[0], ball2[1], ball2[2], ball2[3]);
  if (collis09Result) {
    incrementX1 = -incrementX1;
    //incrementY1 = -incrementY1;
    incrementX2 = -incrementX2;
    //incrementY2 = -incrementY2;
    collis09Result = false;
  }
  if (collis01Result) {
    incrementX1 = 1;
    collis01Result = false;
  }
  if (collis02Result) {
    incrementX1 = -1;
    collis02Result = false;
  }
  if (collis03Result) {
    incrementY1 = 1;
    collis03Result = false;
  }
  if (collis04Result) {
    incrementY1 = -1;
    collis04Result = false;
  }
  if (collis05Result) {
    incrementX2 = 1;
    collis05Result = false;
  }
  if (collis06Result) {
    incrementX2 = -1;
    collis06Result = false;
  }
  if (collis07Result) {
    incrementY2 = 1;
    collis07Result = false;
  }
  if (collis08Result) {
    incrementY2 = -1;
    collis08Result = false;
  }
}

void moveBall() {
  ball1[0] = ball1[0] + incrementX1;
  ball1[1] = ball1[1] + incrementY1;
  ball2[0] = ball2[0] + incrementX2;
  ball2[1] = ball2[1] + incrementY2;
}

void drawBall() {
  oled.clear();
  oled.rect(object1[0], object1[1], object1[2], object1[3], OLED_STROKE);
  oled.rect(object2[0], object2[1], object2[2], object2[3], OLED_STROKE);
  oled.rect(object3[0], object3[1], object3[2], object3[3], OLED_STROKE);
  oled.rect(object4[0], object4[1], object4[2], object4[3], OLED_STROKE);
 //oled.rect(ball1[0], ball1[1], ball1[0] + ball1[2], ball1[1] + ball1[3], OLED_STROKE);
  //oled.rect(ball2[0], ball2[1], ball2[0] + ball2[2], ball2[1] + ball2[3], OLED_STROKE);
  oled.circle(ball1[0] + (ball1[2] / 2), ball1[1] + (ball1[2] / 2), ball1[2], OLED_STROKE);
  oled.circle(ball2[0] + (ball2[2] / 2), ball2[1] + (ball2[2] / 2), ball2[2], OLED_STROKE);
  oled.update();
}
