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

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "notes.h"

#define ONBOARD_LED_PIN  LED_BUILTIN

#define RGB_LED_RED (0)
#define RGB_LED_GREEN (1)
#define RGB_LED_BLUE (22)
#define BUTTON_PIN_1 (15)
#define BUTTON_PIN_2 (17)
#define BUTTON_PIN_3 (2)
#define BUTTON_PIN_4 (3)
#define ULTRASONIC_TRIG_PIN (6)
#define ULTRASONIC_ECHO_PIN (7)
Adafruit_ST7789 tft = Adafruit_ST7789(9, 8, 11, 10, 12);
#define PIEZO_BUZZER_PIN (16)
#define MOTOR_CNTRL_PIN_1 (18)
#define MOTOR_CNTRL_PIN_2 (19)
#define MOTOR_CNTRL_PIN_3 (20)
#define MOTOR_CNTRL_PIN_4 (21)
#define MOTOR_ENABLE_PIN_1 (27)
#define MOTOR_ENABLE_PIN_2 (26)
#define RANDOM_SEED_PIN (28)

//------- tone generation --------
void playNote(int frequency, int duration, bool hold = false, bool measure = true) {
  (void) measure;
  if (hold) {
    // For a note that's held play it a little longer than the specified duration
    // so it blends into the next tone (but there's still a small delay to
    // hear the next note).
    tone(PIEZO_BUZZER_PIN, frequency, duration + duration / 32);
  } else {
    // For a note that isn't held just play it for the specified duration.
    tone(PIEZO_BUZZER_PIN, frequency, duration);
  }
  delay(duration + duration / 16);
}
//--------------------------

float duration_us, distance_cm;
bool buttonState1 = HIGH;
bool buttonState2 = HIGH;
bool buttonState3 = HIGH;
bool buttonState4 = HIGH;
bool robotState = false;
bool obstacleDetected = false;
bool turnDirection = false;
bool rainbowActive = false;
bool danceActivation = false;
bool forwardActivation = false;
bool flipFlop = false;
uint8_t eyeMoodCurrent = 1;
uint8_t eyeMoodOld = 0;
uint8_t mouthMoodCurrent = 1;
uint8_t mouthMoodOld = 0;
uint8_t hornMoodCurrent = 1;
uint8_t hornMoodOld = 0;
uint8_t turnCount = 0;
uint8_t robotMovementCurrent = 1;
uint8_t robotMovementOld = 0;


void setup() {
  setupDisplay();
  setupMotors();
  setupButtons();
  setupUltrasonic();
  setupLeds();
  testLeds();
  testBuzzer();
  setupBackground();
}


void setupDisplay() {
  tft.init(240, 320);
  tft.setRotation(0);
  tft.fillScreen(ST77XX_BLACK);
  delay(250);
}

void setupMotors() {
  pinMode(MOTOR_CNTRL_PIN_1, OUTPUT);
  pinMode(MOTOR_CNTRL_PIN_2, OUTPUT);
  pinMode(MOTOR_CNTRL_PIN_3, OUTPUT);
  pinMode(MOTOR_CNTRL_PIN_4, OUTPUT);
  pinMode(MOTOR_ENABLE_PIN_1, OUTPUT);
  pinMode(MOTOR_ENABLE_PIN_2, OUTPUT);
  delay(250);
}

void setupButtons() {
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  pinMode(BUTTON_PIN_4, INPUT_PULLUP);
  delay(250);
}

void setupUltrasonic() {
  pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  delay(250);
}

void setupLeds() {
  pinMode(ONBOARD_LED_PIN, OUTPUT);
  pinMode(RGB_LED_RED, OUTPUT);
  pinMode(RGB_LED_GREEN, OUTPUT);
  pinMode(RGB_LED_BLUE, OUTPUT);
  delay(250);
}

void setupRandomPins() {
  randomSeed(analogRead(RANDOM_SEED_PIN));
  delay(250);
}

void testLeds() {
  delay(250);
  digitalWrite(ONBOARD_LED_PIN, HIGH);
  delay(2000);
  digitalWrite(ONBOARD_LED_PIN, LOW);
  digitalWrite(RGB_LED_RED, HIGH);
  delay(2000);
  digitalWrite(RGB_LED_RED, LOW);
  digitalWrite(RGB_LED_GREEN, HIGH);
  delay(2000);
  digitalWrite(RGB_LED_GREEN, LOW);
  digitalWrite(RGB_LED_BLUE, HIGH);
  delay(2000);
  digitalWrite(RGB_LED_BLUE, LOW);
}

void testBuzzer() {
  playNote(200, SIXTEENTH, false);
  delay(250);
}

void setupBackground() {
  delay(250);
  tft.fillTriangle(20, 20, 40, 80, 90, 30, ST77XX_MAGENTA);
  tft.fillTriangle(220, 20, 200, 80, 150, 30, ST77XX_MAGENTA);
  tft.fillTriangle(25, 25, 40, 80, 90, 30, ST77XX_BLACK);
  tft.fillTriangle(215, 25, 200, 80, 150, 30, ST77XX_BLACK);
  tft.fillCircle(120, 120, 100, ST77XX_MAGENTA);
  tft.fillRoundRect(70, 180, 100, 110, 30, ST77XX_MAGENTA);
  tft.fillCircle(165, 120, 40, ST77XX_BLACK);
  tft.fillCircle(75, 120, 40, ST77XX_BLACK);
  tft.fillCircle(100, 200, 18, ST77XX_BLACK);
  tft.fillCircle(140, 200, 18, ST77XX_BLACK);
  tft.fillCircle(100, 210, 18, ST77XX_MAGENTA);
  tft.fillCircle(140, 210, 18, ST77XX_MAGENTA);
}

void loop() {
  checkButtons();
  lightHorn();
  renderDisplay();
  moveRobot();
  checkRainbow();
}

void pingDistance() {
  digitalWrite(ULTRASONIC_TRIG_PIN, HIGH);  // generate 10-microsecond pulse to TRIG pin
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIG_PIN, LOW);
  duration_us = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);  // measure duration of pulse from ECHO pin
  distance_cm = 0.017 * duration_us;   // calculate the distance
  if (distance_cm > 2 && distance_cm < 50) {
    robotStop();
    mouthMoodCurrent = 4;
    hornMoodCurrent = 2;
    hornRed();
    robotSqueek();
    delay(250);
    robotTurn();
    forwardActivation = true;
    hornMagenta();
  } else {
    mouthMoodCurrent = 1;
    hornMoodCurrent = 1;
  }
}

void checkButtons() {
  buttonState1 = digitalRead(BUTTON_PIN_1);
  buttonState2 = digitalRead(BUTTON_PIN_2);
  buttonState3 = digitalRead(BUTTON_PIN_3);
  buttonState4 = digitalRead(BUTTON_PIN_4);
  if (buttonState1 == LOW) {                  //Button 1 pressed, do a thing
    robotMovementCurrent = 1;
    rainbowActive = false;
    forwardActivation = false;
  }
  if (buttonState2 == LOW) {                  //Button 3 pressed, do a thing
    robotMovementCurrent = 2;
    forwardActivation = false;
    danceActivation = true;
  }
  if (buttonState3 == LOW) {                  //Button 2 pressed, do a thing
    robotMovementCurrent = 3;
    forwardActivation = true;
    danceActivation = false;
    rainbowActive = false;
    hornMagenta();
  }
  if (buttonState4 == LOW) {                  //Button 4 pressed, do a thing
    rainbowActive = true;
    robotMovementCurrent = 4;
    forwardActivation = false;
    danceActivation = false;
  }
}

void lightHorn() {
  if (hornMoodCurrent != hornMoodOld) {
    if (hornMoodCurrent == 1) {
      hornOff();
    }
    else if (hornMoodCurrent == 2) {
      hornRed();
    }
    else if (hornMoodCurrent == 3) {
      hornGreen();
    }
    else if (hornMoodCurrent == 4) {
      hornBlue();
    }
    else if (hornMoodCurrent == 5) {
      hornCyan();
    }
    else if (hornMoodCurrent == 6) {
      hornMagenta();
    }
    else if (hornMoodCurrent == 7) {
      hornYellow();
    }
    hornMoodOld = hornMoodCurrent;
  }
}

void renderDisplay() {
  if (eyeMoodCurrent != eyeMoodOld) {
    if (eyeMoodCurrent == 1) {
      renderEraseEyes();
      renderEyesStraight();
    }
    else if (eyeMoodCurrent == 2) {
      renderEraseEyes();
      renderEyesLeft();
    }
    else if (eyeMoodCurrent == 3) {
      renderEraseEyes();
      renderEyesRight();
    }
    eyeMoodOld = eyeMoodCurrent;
  }
  if (mouthMoodCurrent != mouthMoodOld) {
    if (mouthMoodCurrent == 1) {
      renderEraseMouth();
      renderSmileMouth();
    }
    else if (mouthMoodCurrent == 2) {
      renderEraseMouth();
      renderSadMouth();
    }
    else if (mouthMoodCurrent == 3) {
      renderEraseMouth();
      renderOohMouth();
    }
    else if (mouthMoodCurrent == 4) {
      renderEraseMouth();
      renderEeshMouth();
    }
    mouthMoodOld = mouthMoodCurrent;
  }
}

void moveRobot() {
  //mode 1 stops robot
  if (robotMovementCurrent != robotMovementOld) {
    if (robotMovementCurrent == 1) {
      eyeMoodCurrent = 1;
      robotStop();
      rainbowActive = true;
      robotMovementOld = robotMovementCurrent;
    } else if (robotMovementCurrent == 4) {
      rainbowActive = false;
    }
  }
  if (robotMovementCurrent == 2) {
    eyeMoodCurrent = 1;
    robotDriveForward();
  } else if (robotMovementCurrent == 3) {
    robotDance();
  }
}

void checkRainbow() {
  if (rainbowActive == true) {
    hornMoodCurrent = hornMoodCurrent + 1;
    if (hornMoodCurrent > 7) {
      hornMoodCurrent = 2;
    }
  }
}

void robotStop() {
  digitalWrite(MOTOR_ENABLE_PIN_1, LOW);
  digitalWrite(MOTOR_ENABLE_PIN_2, LOW);
}

void robotTurn() {
  if (turnDirection == true) {
    eyeMoodCurrent = 2;
    robotTurnLeft();
  } else {
    eyeMoodCurrent = 3;
    robotTurnRight();
  }
  turnCount = turnCount + 1;
  if (turnCount > 4) {
    turnDirection = !turnDirection;
    turnCount = 0;
  }
}

void robotTurnLeft() {
  digitalWrite(MOTOR_ENABLE_PIN_1, LOW);
  digitalWrite(MOTOR_ENABLE_PIN_2, LOW);
  digitalWrite(MOTOR_CNTRL_PIN_1, LOW);
  digitalWrite(MOTOR_CNTRL_PIN_2, HIGH);
  digitalWrite(MOTOR_CNTRL_PIN_3, LOW);
  digitalWrite(MOTOR_CNTRL_PIN_4, HIGH);
  digitalWrite(MOTOR_ENABLE_PIN_1, HIGH);
  digitalWrite(MOTOR_ENABLE_PIN_2, HIGH);
  delay(250);
  digitalWrite(MOTOR_ENABLE_PIN_1, LOW);
  digitalWrite(MOTOR_ENABLE_PIN_2, LOW);
}

void robotTurnRight() {
  digitalWrite(MOTOR_ENABLE_PIN_1, LOW);
  digitalWrite(MOTOR_ENABLE_PIN_2, LOW);
  digitalWrite(MOTOR_CNTRL_PIN_1, HIGH);
  digitalWrite(MOTOR_CNTRL_PIN_2, LOW);
  digitalWrite(MOTOR_CNTRL_PIN_3, HIGH);
  digitalWrite(MOTOR_CNTRL_PIN_4, LOW);
  digitalWrite(MOTOR_ENABLE_PIN_1, HIGH);
  digitalWrite(MOTOR_ENABLE_PIN_2, HIGH);
  delay(250);
  digitalWrite(MOTOR_ENABLE_PIN_1, LOW);
  digitalWrite(MOTOR_ENABLE_PIN_2, LOW);
}

void robotDriveForward() {
  pingDistance();
  if (forwardActivation == true) {
    forwardActivation = false;
    digitalWrite(MOTOR_ENABLE_PIN_1, LOW);
    digitalWrite(MOTOR_ENABLE_PIN_2, LOW);
    digitalWrite(MOTOR_CNTRL_PIN_1, LOW);
    digitalWrite(MOTOR_CNTRL_PIN_2, HIGH);
    digitalWrite(MOTOR_CNTRL_PIN_3, HIGH);
    digitalWrite(MOTOR_CNTRL_PIN_4, LOW);
    digitalWrite(MOTOR_ENABLE_PIN_1, HIGH);
    digitalWrite(MOTOR_ENABLE_PIN_2, HIGH);
  }
}

void robotDance() {
  robotSpeak();
  if (flipFlop == true) {
    eyeMoodCurrent = 1;
    robotTurnLeft();
  } else {
    eyeMoodCurrent = 2;
    robotTurnRight();
  }
  flipFlop = !flipFlop;
}

void robotSpeak() {
  playNote(random(100, 4000), SIXTEENTH, false);
  playNote(random(100, 4000), SIXTEENTH, false);
  playNote(random(100, 4000), SIXTEENTH, false);
  playNote(random(100, 4000), SIXTEENTH, false);
  playNote(random(100, 4000), SIXTEENTH, false);
}

void robotSqueek() {
  playNote(1000, SIXTEENTH, false);
  playNote(250, SIXTEENTH, false);
}

void renderEraseEyes() {
  tft.fillCircle(165, 120, 40, ST77XX_BLACK);
  tft.fillCircle(75, 120, 40, ST77XX_BLACK);
}

void renderEyesStraight() {
  tft.fillCircle(165, 120, 20, ST77XX_CYAN);
  tft.fillCircle(75, 120, 20, ST77XX_CYAN);
  tft.fillCircle(165, 120, 10, ST77XX_BLACK);
  tft.fillCircle(75, 120, 10, ST77XX_BLACK);
}

void renderEyesLeft() {
  tft.fillCircle(145, 120, 20, ST77XX_CYAN);
  tft.fillCircle(55, 120, 20, ST77XX_CYAN);
  tft.fillCircle(145, 120, 10, ST77XX_BLACK);
  tft.fillCircle(55, 120, 10, ST77XX_BLACK);
}

void renderEyesRight() {
  tft.fillCircle(185, 120, 20, ST77XX_CYAN);
  tft.fillCircle(95, 120, 20, ST77XX_CYAN);
  tft.fillCircle(185, 120, 10, ST77XX_BLACK);
  tft.fillCircle(95, 120, 10, ST77XX_BLACK);
}

void renderEraseMouth() {
  tft.drawRoundRect(80, 230, 80, 40, 10, ST77XX_MAGENTA);
}

void renderSadMouth() {
  tft.fillRoundRect(80, 240, 80, 40, 50, ST77XX_BLACK);
  tft.fillRoundRect(80, 250, 80, 40, 50, ST77XX_MAGENTA);
}

void renderSmileMouth() {
  tft.fillRoundRect(80, 230, 80, 40, 50, ST77XX_BLACK);
  tft.fillRoundRect(80, 215, 80, 40, 50, ST77XX_MAGENTA);
}

void renderOohMouth() {
  tft.fillRoundRect(80, 230, 80, 40, 40, ST77XX_BLACK);
}

void renderEeshMouth() {
  tft.fillRoundRect(80, 230, 80, 40, 10, ST77XX_BLACK);
}

void hornOff() {
  digitalWrite(RGB_LED_RED, LOW);
  digitalWrite(RGB_LED_GREEN, LOW);
  digitalWrite(RGB_LED_BLUE, LOW);
}

void hornRed() {
  hornOff();
  digitalWrite(RGB_LED_RED, LOW);
  digitalWrite(RGB_LED_GREEN, LOW);
  digitalWrite(RGB_LED_BLUE, LOW);
  digitalWrite(RGB_LED_RED, HIGH);
}

void hornGreen() {
  hornOff();
  digitalWrite(RGB_LED_RED, LOW);
  digitalWrite(RGB_LED_GREEN, LOW);
  digitalWrite(RGB_LED_BLUE, LOW);
  digitalWrite(RGB_LED_GREEN, HIGH);
}

void hornBlue() {
  hornOff();
  digitalWrite(RGB_LED_RED, LOW);
  digitalWrite(RGB_LED_GREEN, LOW);
  digitalWrite(RGB_LED_BLUE, LOW);
  digitalWrite(RGB_LED_BLUE, HIGH);
}

void hornCyan() {
  hornOff();
  digitalWrite(RGB_LED_RED, LOW);
  digitalWrite(RGB_LED_GREEN, LOW);
  digitalWrite(RGB_LED_BLUE, LOW);
  digitalWrite(RGB_LED_GREEN, HIGH);
  digitalWrite(RGB_LED_BLUE, HIGH);
}

void hornMagenta() {
  hornOff();
  digitalWrite(RGB_LED_RED, LOW);
  digitalWrite(RGB_LED_GREEN, LOW);
  digitalWrite(RGB_LED_BLUE, LOW);
  digitalWrite(RGB_LED_RED, HIGH);
  digitalWrite(RGB_LED_BLUE, HIGH);
}

void hornYellow() {
  hornOff();
  digitalWrite(RGB_LED_RED, LOW);
  digitalWrite(RGB_LED_GREEN, LOW);
  digitalWrite(RGB_LED_BLUE, LOW);
  digitalWrite(RGB_LED_RED, HIGH);
  digitalWrite(RGB_LED_GREEN, HIGH);
}
