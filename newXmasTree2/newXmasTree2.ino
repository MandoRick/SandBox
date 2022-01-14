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

#define mydebug 1     // 1 ON 0 OFF
#if mydebug == 1
#define debugln(x) Serial.println(x)
#define debug(x) Serial.print(x)
#else
#define debugln(x)
#define debug(x)
#endif
#define ledPin  LED_BUILTIN

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

#define ColorLedOn HIGH
#define ColorLedOff LOW
#define ColumnOn LOW
#define ColumnOff HIGH

const byte redPins[] = {2, 5, 8, 11};
const byte greenPins[] = {3, 6, 9, 12};
const byte bluePins[] = {4, 7, 10, 13};
const byte blackPins[] = {19, 14, 16, 17, 18, 15};

uint16_t delayUpperLimit = 500;
uint16_t animationDelay = 1;
uint8_t blackPinNumber = 0;
uint8_t animationSelection = 0;

void setup() {
  delay(2000);
  Serial.begin(115200);
  debugln("Serial started");
  versionPrint();
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i = 0; i <= 3; i++) {
    pinMode(redPins[i], OUTPUT);
    pinMode(greenPins[i], OUTPUT);
    pinMode(bluePins[i], OUTPUT);

  }
  for (int i = 0; i <= 5; i++) {
    pinMode(blackPins[i], OUTPUT);
  }
  randomSeed(analogRead(A0));
}

void loop() {
  animationSelection = random(12);
  if (animationSelection == 0) {
    swirlAnimation();
  } else if (animationSelection == 1) {
    paintAnimation1();
  } else if (animationSelection == 2) {
    chaseAnimation1();
  } else if (animationSelection == 3) {
    randomSlowAnimation();
  } else if (animationSelection == 4) {
    spearAnimation1();
  } else if (animationSelection == 5) {
    stackAnimation();
  } else if (animationSelection == 6) {
    randomFastAnimation();
  } else if (animationSelection == 7) {
    paintAnimation2();
  } else if (animationSelection == 8) {
    paintAnimation3();
  } else if (animationSelection == 9) {
    chaseAnimation2();
  } else if (animationSelection == 10) {
    chaseAnimation3();
  } else if (animationSelection == 11) {
    spearAnimation2();
  }
}

void swirlAnimation() {
  redSwirl();
  reverseRedSwirl();
  greenSwirl();
  reverseGreenSwirl();
  blueSwirl();
  reverseBlueSwirl();
}

void paintAnimation1() {
  redPaint();
}

void paintAnimation2() {
  greenPaint();
}

void paintAnimation3() {
  bluePaint();
}

void chaseAnimation1() {
  redSwirl();
  ledRowsOff();
  ledColumnsOff(); 
}

void chaseAnimation2() {
  greenSwirl();
  ledRowsOff();
  ledColumnsOff(); 
}

void chaseAnimation3() {  
  blueSwirl();
  ledRowsOff();
  ledColumnsOff();
}

void stackAnimation() {
  delayUpperLimit = 500;
  for (int h = 5; h >= 0; h--) {
    digitalWrite(blackPins[h], ColumnOn);
  }
  for (int i = 0; i <= random(4); i++) {
    digitalWrite(redPins[3], ColorLedOn);
    callDelay();
    delay(animationDelay);
    digitalWrite(greenPins[2], ColorLedOn);
    callDelay();
    delay(animationDelay);
    digitalWrite(redPins[1], ColorLedOn);
    callDelay();
    delay(animationDelay);
    digitalWrite(greenPins[0], ColorLedOn);
    callDelay();
    delay(animationDelay);
    ledRowsOff();
    digitalWrite(greenPins[3], ColorLedOn);
    callDelay();
    delay(animationDelay);
    digitalWrite(redPins[2], ColorLedOn);
    callDelay();
    delay(animationDelay);
    digitalWrite(greenPins[1], ColorLedOn);
    callDelay();
    delay(animationDelay);
    digitalWrite(redPins[0], ColorLedOn);
    callDelay();
    delay(animationDelay);
    ledRowsOff();
  }
  ledRowsOff();
  ledColumnsOff();
}

void callDelay() {
  animationDelay = random(1, delayUpperLimit);
}

void randomSlowAnimation() {
  delayUpperLimit = 1000;
  for (int h = 0; h <= 25; h++) {
    digitalWrite(blackPins[random(6)], ColumnOn);
    digitalWrite(redPins[random(4)], ColorLedOn);
    digitalWrite(greenPins[random(4)], ColorLedOn);
    digitalWrite(bluePins[random(4)], ColorLedOn);
    callDelay();
    delay(animationDelay);
    digitalWrite(blackPins[random(6)], ColumnOff);
    digitalWrite(redPins[random(4)], ColorLedOff);
    digitalWrite(greenPins[random(4)], ColorLedOff);
    digitalWrite(bluePins[random(4)], ColorLedOff);
  }
  ledRowsOff();
  ledColumnsOff();
}

void randomFastAnimation() {
  delayUpperLimit = 25;
  for (int h = 0; h <= 500; h++) {
    digitalWrite(blackPins[random(6)], ColumnOn);
    digitalWrite(redPins[random(4)], ColorLedOn);
    digitalWrite(greenPins[random(4)], ColorLedOn);
    digitalWrite(bluePins[random(4)], ColorLedOn);
    callDelay();
    delay(animationDelay);
    digitalWrite(blackPins[random(6)], ColumnOff);
    digitalWrite(redPins[random(4)], ColorLedOff);
    digitalWrite(greenPins[random(4)], ColorLedOff);
    digitalWrite(bluePins[random(4)], ColorLedOff);
  }
  ledRowsOff();
  ledColumnsOff();
}

void spearAnimation1() {
  delayUpperLimit = 100;
  for (int h = 0; h <= 5; h++) {
    digitalWrite(blackPins[h], ColumnOn);
    for (int h = 0; h <= 3; h++) {
      digitalWrite(redPins[h], ColorLedOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(redPins[h], ColorLedOff);
    }
    for (int h = 0; h <= 3; h++) {
      digitalWrite(greenPins[h], ColorLedOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(greenPins[h], ColorLedOff);
    }
    for (int h = 0; h <= 3; h++) {
      digitalWrite(bluePins[h], ColorLedOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(bluePins[h], ColorLedOff);
    }
    digitalWrite(blackPins[h], ColumnOff);
  } 
}

void spearAnimation2() {
  delayUpperLimit = 100;
  for (int h = 5; h >= 0; h--) {
    digitalWrite(blackPins[h], ColumnOn);
    for (int h = 3; h >= 0; h--) {
      digitalWrite(redPins[h], ColorLedOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(redPins[h], ColorLedOff);
    }
    for (int h = 3; h >= 0; h--) {
      digitalWrite(greenPins[h], ColorLedOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(greenPins[h], ColorLedOff);
    }
    for (int h = 3; h >= 0; h--) {
      digitalWrite(bluePins[h], ColorLedOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(bluePins[h], ColorLedOff);
    }
    digitalWrite(blackPins[h], ColumnOff);
  }
}

void ledRowsOff() {
  for (int h = 0; h <= 3; h++) {
    digitalWrite(redPins[h], ColorLedOff);
    digitalWrite(greenPins[h], ColorLedOff);
    digitalWrite (bluePins[h], ColorLedOff);
  }
}

void ledColumnsOff() {
  for (int h = 0; h <= 5; h++) {
    digitalWrite(blackPins[h], ColumnOff);
  }
}

void ledColumnsOn() {
  for (int h = 0; h <= 5; h++) {
    digitalWrite(blackPins[h], ColumnOn);
  }
}

void redSwirl() {
  delayUpperLimit = 50;
  for (int i = 0; i <= 3; i++) {
    digitalWrite(redPins[i], ColorLedOn);
    for (int j = 0; j <= 5; j++) {
      digitalWrite(blackPins[j], ColumnOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(blackPins[j], ColumnOff);
    }
    digitalWrite(redPins[i], ColorLedOff);
  }
}

void reverseRedSwirl() {
  delayUpperLimit = 50;
  for (int k = 3; k >= 0; k--) {
    digitalWrite(redPins[k], ColorLedOn);
    for (int l = 5; l >= 0; l--) {
      digitalWrite(blackPins[l], ColumnOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(blackPins[l], ColumnOff);
    }
    digitalWrite(redPins[k], ColorLedOff);
  }
}

void greenSwirl() {
  delayUpperLimit = 50;
  for (int i = 0; i <= 3; i++) {
    digitalWrite(greenPins[i], ColorLedOn);
    for (int j = 0; j <= 5; j++) {
      digitalWrite(blackPins[j], ColumnOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(blackPins[j], ColumnOff);
    }
    digitalWrite(greenPins[i], ColorLedOff);
  }
}

void reverseGreenSwirl() {
  delayUpperLimit = 50;
  for (int k = 3; k >= 0; k--) {
    digitalWrite(greenPins[k], ColorLedOn);
    for (int l = 5; l >= 0; l--) {
      digitalWrite(blackPins[l], ColumnOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(blackPins[l], ColumnOff);
    }
    digitalWrite(greenPins[k], ColorLedOff);
  }
}

void blueSwirl() {
  delayUpperLimit = 50;
  for (int i = 0; i <= 3; i++) {
    digitalWrite(bluePins[i], ColorLedOn);
    for (int j = 0; j <= 5; j++) {
      digitalWrite(blackPins[j], ColumnOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(blackPins[j], ColumnOff);
    }
    digitalWrite(bluePins[i], ColorLedOff);
  }
}

void reverseBlueSwirl() {
  delayUpperLimit = 50;
  for (int k = 3; k >= 0; k--) {
    digitalWrite(bluePins[k], ColorLedOn);
    for (int l = 5; l >= 0; l--) {
      digitalWrite(blackPins[l], ColumnOn);
      callDelay();
      delay(animationDelay);
      digitalWrite(blackPins[l], ColumnOff);
    }
    digitalWrite(bluePins[k], ColorLedOff);
  }
}

void redPaint() {
  delayUpperLimit = 100;
  for (int i = 0; i <= 3; i++) {
    digitalWrite(redPins[i], ColorLedOn);
    for (int j = 0; j <= 5; j++) {
      digitalWrite(blackPins[j], ColumnOn);
      callDelay();
      delay(animationDelay);
    }
    for (int j = 5; j >= 0; j--) {
      digitalWrite(blackPins[j], ColumnOff);
      callDelay();
      delay(animationDelay);
    }
    digitalWrite(redPins[i], ColorLedOff);
  }
}

void greenPaint() {
  delayUpperLimit = 100;
  for (int i = 0; i <= 3; i++) {
    digitalWrite(redPins[i], ColorLedOff);
    digitalWrite(greenPins[i], ColorLedOn);
    for (int j = 0; j <= 5; j++) {
      digitalWrite(blackPins[j], ColumnOn);
      callDelay();
      delay(animationDelay);
    }
    for (int j = 5; j >= 0; j--) {
      digitalWrite(blackPins[j], ColumnOff);
      callDelay();
      delay(animationDelay);
    }
    digitalWrite(greenPins[i], ColorLedOff);
  }
}

void bluePaint() {
  delayUpperLimit = 100;
  for (int i = 0; i <= 3; i++) {
    digitalWrite(greenPins[i], ColorLedOff);
    digitalWrite(bluePins[i], ColorLedOn);
    for (int j = 0; j <= 5; j++) {
      digitalWrite(blackPins[j], ColumnOn);
      callDelay();
      delay(animationDelay);
    }
    for (int j = 5; j >= 0; j--) {
      digitalWrite(blackPins[j], ColumnOff);
      callDelay();
      delay(animationDelay);
    }
    digitalWrite(bluePins[i], ColorLedOff);
  }
}
