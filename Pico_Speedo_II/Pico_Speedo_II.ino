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

#define PIN_QTY (3)
#define WHEEL_DIAMETER 300  // wheel diameter in mm

uint8_t HALL_PINS[PIN_QTY] = { 16, 17, 18 };
volatile bool hallFlags[PIN_QTY] = { 0, 0, 0 };
uint16_t rotationCount = 0;
float currentRpm = 0;
float currentSpeed = 0;
float currentDistance = 0;
float wheelCircumference = WHEEL_DIAMETER * PI;  // Circumference  = Diameter * π

unsigned long currentMillis, previousMillis;

void setup() {
  Serial.begin(115200);
  for (uint8_t i = 0; i < PIN_QTY; i++) {
    pinMode(HALL_PINS[i], INPUT_PULLUP);
    delay(25);
  }
  attachInterrupt(digitalPinToInterrupt(HALL_PINS[0]), hallInterrupt0, FALLING);
  attachInterrupt(digitalPinToInterrupt(HALL_PINS[1]), hallInterrupt1, FALLING);
  attachInterrupt(digitalPinToInterrupt(HALL_PINS[2]), hallInterrupt2, FALLING);
  delay(500);
  previousMillis = millis();
}

void loop() {
  checkFlags();
}

void hallInterrupt0() {
  hallFlags[0] = 1;
}

void hallInterrupt1() {
  hallFlags[1] = 1;
}

void hallInterrupt2() {
  hallFlags[2] = 1;
}

void checkFlags() {
  if (hallFlags[0] && hallFlags[1] && hallFlags[2]) {
    hallFlags[0] = 0;
    hallFlags[1] = 0;
    hallFlags[2] = 0;
    rotationCount++;
    calculateRpmAndSpeed();
  }
}

void calculateRpmAndSpeed() {
  currentMillis = millis();
  unsigned long rotationTime = currentMillis - previousMillis;
  previousMillis = currentMillis;
  currentRpm = 60000.0 / rotationTime;                                // Calculate RPM
  currentDistance = (rotationCount * wheelCircumference) * 0.000001;  // Distance in km
  if (rotationTime != 0) {
    currentSpeed = (wheelCircumference / 1000) / (rotationTime / 60000.0);  // Speed in km/h
  }
  Serial.print("RPM: ");
  Serial.println(currentRpm);
  Serial.print("Speed: ");
  Serial.print(currentSpeed);
  Serial.println(" km/h");
  Serial.print("Distance: ");
  Serial.print(currentDistance);
  Serial.println(" km");
}
