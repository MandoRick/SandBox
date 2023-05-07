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

#include <GyverStepper.h>
#include <GyverOLED.h>
#include <GyverFIFO.h>

#define PIN_QTY (6)
#define SENSOR_QTY (3)
#define STEPPER_RES (2048)
#define SPEED_LIMIT (30000)

uint8_t SENSOR_PINS[PIN_QTY] = {10, 11, 12, 13, 14, 15}; //yellow, green, yellow, green, yellow, green
uint8_t STEPPER_MOTOR_PINS[PIN_QTY] = {2, 3, 6, 7, 8, 9}; //blue, green, blue, green, blue, green
uint8_t MICROSWITCH_PINS[PIN_QTY] = {16, 17, 18, 19, 20, 21};
int16_t STEPPER_SPEED[SENSOR_QTY] = {0, 0, 0};
int16_t OLD_STEPPER_SPEED[SENSOR_QTY] = {0, 0, 0};
volatile int16_t ROTATION_COUNTS[SENSOR_QTY] = {0, 0, 0}; // variable to store the value coming from the direction sensor
volatile int16_t OLD_ROTATION_COUNTS[SENSOR_QTY] = {0, 0, 0};
volatile bool ROTATION_FLAGS[PIN_QTY] = {false, false, false, false, false, false};
volatile bool STOPPER_FLAGS[PIN_QTY] = {false, false, false, false, false, false};
int16_t BUFF_READ[SENSOR_QTY] = { 0, 0, 0 };

//GStepper<STEPPER2WIRE> name(resolution, blue, green)
//GStepper<STEPPER2WIRE> name(resolution, stepPin, dirPin)
GStepper<STEPPER2WIRE> STEPPER_X(STEPPER_RES, STEPPER_MOTOR_PINS[0], STEPPER_MOTOR_PINS[1]);
GStepper<STEPPER2WIRE> STEPPER_Y(STEPPER_RES, STEPPER_MOTOR_PINS[2], STEPPER_MOTOR_PINS[3]);
GStepper<STEPPER2WIRE> STEPPER_Z(STEPPER_RES, STEPPER_MOTOR_PINS[4], STEPPER_MOTOR_PINS[5]);
GyverFIFO<int16_t, 16> FIFO_BUFF_0;
GyverFIFO<int16_t, 16> FIFO_BUFF_1;
GyverFIFO<int16_t, 16> FIFO_BUFF_2;
GyverOLED<SSD1306_128x64, OLED_BUFFER> OLED;

void setup() {
  Serial.begin(115200);
  delay(50);
  for (uint8_t i = 0; i < PIN_QTY; i++) {
    pinMode(SENSOR_PINS[i], INPUT_PULLUP);
    delay(5);
  }
  for (uint8_t i = 0; i < PIN_QTY; i++) {
    pinMode(MICROSWITCH_PINS[i], INPUT_PULLUP);
    delay(5);
  }
  attachInterrupt(digitalPinToInterrupt(MICROSWITCH_PINS[0]), microswitchPinInterrupt0, RISING);
  attachInterrupt(digitalPinToInterrupt(MICROSWITCH_PINS[1]), microswitchPinInterrupt1, RISING);
  attachInterrupt(digitalPinToInterrupt(MICROSWITCH_PINS[2]), microswitchPinInterrupt2, RISING);
  attachInterrupt(digitalPinToInterrupt(MICROSWITCH_PINS[3]), microswitchPinInterrupt3, RISING);
  attachInterrupt(digitalPinToInterrupt(MICROSWITCH_PINS[4]), microswitchPinInterrupt4, RISING);
  attachInterrupt(digitalPinToInterrupt(MICROSWITCH_PINS[5]), microswitchPinInterrupt5, RISING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PINS[0]), sensorPinInterrupt0, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PINS[2]), sensorPinInterrupt1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSOR_PINS[4]), sensorPinInterrupt2, FALLING);
  delay(5);
  STEPPER_X.setRunMode(KEEP_SPEED);
  STEPPER_Y.setRunMode(KEEP_SPEED);
  STEPPER_Z.setRunMode(KEEP_SPEED);
  delay(5);
  STEPPER_X.setSpeedDeg(STEPPER_SPEED[0]);
  STEPPER_Y.setSpeedDeg(STEPPER_SPEED[1]);
  STEPPER_Z.setSpeedDeg(STEPPER_SPEED[2]);
  delay(5);
}

void loop() {
  parseInputData();
  setStepperSpeed();
  moveStepper();
  printValues();
}

void sensorPinInterrupt0() {
  if (digitalRead(SENSOR_PINS[0]) == digitalRead(SENSOR_PINS[1])) {
    ROTATION_FLAGS[0] = true;
  } else {
    ROTATION_FLAGS[1] = true;
  }
}

void sensorPinInterrupt1() {
  if (digitalRead(SENSOR_PINS[2]) == digitalRead(SENSOR_PINS[3])) {
    ROTATION_FLAGS[2] = true;
  } else {
    ROTATION_FLAGS[3] = true;
  }
}

void sensorPinInterrupt2() {
  if (digitalRead(SENSOR_PINS[4]) == digitalRead(SENSOR_PINS[5])) {
    ROTATION_FLAGS[4] = true;
  } else {
    ROTATION_FLAGS[5] = true;
  }
}

void microswitchPinInterrupt0() {
  STOPPER_FLAGS[0] = true;
}

void microswitchPinInterrupt1() {
  STOPPER_FLAGS[1] = true;
}

void microswitchPinInterrupt2() {
  STOPPER_FLAGS[2] = true;
}

void microswitchPinInterrupt3() {
  STOPPER_FLAGS[3] = true;
}

void microswitchPinInterrupt4() {
  STOPPER_FLAGS[4] = true;
}

void microswitchPinInterrupt5() {
  STOPPER_FLAGS[5] = true;
}

void parseInputData() {
  if (ROTATION_FLAGS[0] == true) {
    ROTATION_COUNTS[0]--;
    ROTATION_FLAGS[0] = false;
  }
  if (ROTATION_FLAGS[1] == true) {
    ROTATION_COUNTS[0]++;
    ROTATION_FLAGS[1] = false;
  }
  if (ROTATION_FLAGS[2] == true) {
    ROTATION_COUNTS[1]--;
    ROTATION_FLAGS[2] = false;
  }
  if (ROTATION_FLAGS[3] == true) {
    ROTATION_COUNTS[1]++;
    ROTATION_FLAGS[3] = false;
  }
  if (ROTATION_FLAGS[4] == true) {
    ROTATION_COUNTS[2]--;
    ROTATION_FLAGS[4] = false;
  }
  if (ROTATION_FLAGS[5] == true) {
    ROTATION_COUNTS[2]++;
    ROTATION_FLAGS[5] = false;
  }
  if (ROTATION_COUNTS[0] < -SPEED_LIMIT) {
    ROTATION_COUNTS[0] = -SPEED_LIMIT;
  } else if (ROTATION_COUNTS[0] > SPEED_LIMIT) {
    ROTATION_COUNTS[0] = SPEED_LIMIT;
  }
  if (ROTATION_COUNTS[1] < -SPEED_LIMIT) {
    ROTATION_COUNTS[1] = -SPEED_LIMIT;
  } else if (ROTATION_COUNTS[1] > SPEED_LIMIT) {
    ROTATION_COUNTS[1] = SPEED_LIMIT;
  }
  if (ROTATION_COUNTS[2] < -SPEED_LIMIT) {
    ROTATION_COUNTS[2] = -SPEED_LIMIT;
  } else if (ROTATION_COUNTS[2] > SPEED_LIMIT) {
    ROTATION_COUNTS[2] = SPEED_LIMIT;
  }
  STEPPER_SPEED[0] = ROTATION_COUNTS[0] / 10;
  STEPPER_SPEED[1] = ROTATION_COUNTS[1] / 10;
  STEPPER_SPEED[2] = ROTATION_COUNTS[2] / 10;
}

void setStepperSpeed() {
  if (STEPPER_SPEED[0] != OLD_STEPPER_SPEED[0]) {
    STEPPER_X.setSpeedDeg(STEPPER_SPEED[0]);
     FIFO_BUFF_0.write(STEPPER_SPEED[0]);
    OLD_STEPPER_SPEED[0] = STEPPER_SPEED[0];
  }
  if (STEPPER_SPEED[1] != OLD_STEPPER_SPEED[1]) {
    STEPPER_Y.setSpeedDeg(STEPPER_SPEED[1]);
    FIFO_BUFF_1.write(STEPPER_SPEED[1]);
    OLD_STEPPER_SPEED[1] = STEPPER_SPEED[1];
  }
  if (STEPPER_SPEED[2] != OLD_STEPPER_SPEED[2]) {
    STEPPER_Z.setSpeedDeg(STEPPER_SPEED[2]);
    FIFO_BUFF_2.write(STEPPER_SPEED[2]);
    OLD_STEPPER_SPEED[2] = STEPPER_SPEED[2];
  }
}

void moveStepper() {
  STEPPER_X.tick();
  STEPPER_Y.tick();
  STEPPER_Z.tick();
}

void printValues() {
  if (ROTATION_COUNTS[0] != OLD_ROTATION_COUNTS[0]) {
    Serial.println("rotationCount1: " + (String)ROTATION_COUNTS[0]);
    Serial.println("stepperSpeed1: " + (String)STEPPER_SPEED[0]);
    OLD_ROTATION_COUNTS[0] = ROTATION_COUNTS[0];
  }
  if (ROTATION_COUNTS[1] != OLD_ROTATION_COUNTS[1]) {
    Serial.println("rotationCount2: " + (String)ROTATION_COUNTS[1]);
    Serial.println("stepperSpeed2: " + (String)STEPPER_SPEED[1]);
    OLD_ROTATION_COUNTS[1] = ROTATION_COUNTS[1];
  }
  if (ROTATION_COUNTS[2] != OLD_ROTATION_COUNTS[2]) {
    Serial.println("rotationCount3: " + (String)ROTATION_COUNTS[2]);
    Serial.println("stepperSpeed3: " + (String)STEPPER_SPEED[2]);
    OLD_ROTATION_COUNTS[2] = ROTATION_COUNTS[2];
  }
  if (STOPPER_FLAGS[0] == true) {
    Serial.println("stopperFlag0: " + (String)STOPPER_FLAGS[0]);
    STOPPER_FLAGS[0] = false;
  }
}

void setup1() {
  OLED.init();
  OLED.clear();
  OLED.update();
}

void loop1() {
  while (FIFO_BUFF_0.available()) {
    BUFF_READ[0] = FIFO_BUFF_0.read();
  }
  while (FIFO_BUFF_1.available()) {
    BUFF_READ[1] = FIFO_BUFF_1.read();
  }
  while (FIFO_BUFF_2.available()) {
    BUFF_READ[2] = FIFO_BUFF_2.read();
  }
  OLED.clear();
  OLED.setScale(2);
  OLED.setCursor(16, 2);
  OLED.print("X");
  OLED.setCursor(60, 2);
  OLED.print("Y");
  OLED.setCursor(102, 2);
  OLED.print("Z");
  OLED.roundRect(2, 40, 42, 62, OLED_STROKE);
  OLED.roundRect(44, 40, 84, 62, OLED_STROKE);
  OLED.roundRect(86, 40, 126, 62, OLED_STROKE);
  OLED.setScale(1);
  OLED.setCursor(7, 6);
  OLED.print(BUFF_READ[0]);
  OLED.setCursor(49, 6);
  OLED.print(BUFF_READ[1]);
  OLED.setCursor(92, 6);
  OLED.print(BUFF_READ[2]);
  OLED.update();
}
