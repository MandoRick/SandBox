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

#include <math.h>
#include <GyverOLED.h>

#define BUTTON_COUNT 2     // Choose button QTY
#define DEBOUNCE_TIME 200  // Choose debounce time

#define OLED_SPEED 800000L
#define DIGIT_QTY 9
#define FLASH_DURATION 25

const uint8_t BUTTON_PIN[BUTTON_COUNT] = { 14, 15 };  // Add button pins as needed
volatile bool buttonFlag[BUTTON_COUNT] = {};
volatile bool jobFlag[BUTTON_COUNT] = {};
unsigned long currentMillis[BUTTON_COUNT] = {};
unsigned long previousMillis[BUTTON_COUNT] = {};

uint32_t fpsTimer;

int8_t digitArray[DIGIT_QTY] = {};
uint8_t digitPosition = 0;
uint32_t finalValue;
uint8_t flashTimer = 0;
bool flasher = false;

GyverOLED<SSH1106_128x64> oled;

void setup() {
  Serial.begin(115200);
  setupButtons();
  prepareArray();
  oled.init();
  Wire.setClock(OLED_SPEED);  
}

void loop() {
  clearDisplay();
  checkButtons();
  checkJobs();
  calculateArray();
  generateFinalNumber();
  renderDisplay();
}

void setupButtons() {
  for (int buttonPin = 0; buttonPin < BUTTON_COUNT; buttonPin++) {
    pinMode(BUTTON_PIN[buttonPin], INPUT);
    buttonFlag[buttonPin] = false;
    jobFlag[buttonPin] = false;
    currentMillis[buttonPin] = 0;
    previousMillis[buttonPin] = 0;
  }
}

void prepareArray() {
  for (int i = 0; i < DIGIT_QTY; i++) {
    digitArray[i] = 0;
  }
}

void clearDisplay() {
  oled.clear();
  oled.home();
  oled.print(1000 / fpsTimer);
  fpsTimer = millis();
}

void checkButtons() {
  for (int buttonPin = 0; buttonPin < BUTTON_COUNT; buttonPin++) {
    currentMillis[buttonPin] = millis();
    if ((unsigned long)(currentMillis[buttonPin] - previousMillis[buttonPin]) >= DEBOUNCE_TIME) {
      buttonFlag[buttonPin] = digitalRead(BUTTON_PIN[buttonPin]);
      if (buttonFlag[buttonPin] == true) {
        buttonFlag[buttonPin] = false;
        jobFlag[buttonPin] = true;
      }
      previousMillis[buttonPin] = currentMillis[buttonPin];
    }
  }
}

void checkJobs() {
  if (jobFlag[0]) {
    jobFlag[0] = false;
    job0();
  }
  if (jobFlag[1]) {
    jobFlag[1] = false;
    job1();
  }
}

void calculateArray() {
  oled.setScale(2);
  for (int i = 0; i < DIGIT_QTY; i++) {
    oled.setCursor(i * 12, 2);
    oled.print(digitArray[i]);
  }
  flashTimer++;
  if (flashTimer > FLASH_DURATION) {
    flasher = !flasher;
    flashTimer = 0;
  }
  if (flasher) {
    oled.line(digitPosition * 12, 35, digitPosition * 12 + 8, 35, 1);
  } else {
    oled.line(digitPosition * 12, 35, digitPosition * 12 + 8, 35, 0);
  }
}

void generateFinalNumber() {
  finalValue = 0;
  for (int i = 0; i < DIGIT_QTY; i++) {
    uint32_t multipliedValue = digitArray[i] * pow(10, DIGIT_QTY - 1 - i);
    finalValue = finalValue + multipliedValue;
  }
  oled.setScale(1);
  oled.setCursor(0, 5);
  oled.print(finalValue);
}

void renderDisplay() {
  oled.update();
  fpsTimer = millis() - fpsTimer;
}

void job0() {
  Serial.println("Doing job 0");
  digitPosition++;
  if (digitPosition > DIGIT_QTY - 1) {
    digitPosition = 0;
  }
}

void job1() {
  Serial.println("Doing job 1");
  uint8_t currentNumber = digitArray[digitPosition];
  currentNumber++;
  if (currentNumber > 9) {
    currentNumber = 0;
  }
  digitArray[digitPosition] = currentNumber;
}
