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

#define debug 1     // 1 ON 0 OFF
#if debug == 1
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

// Rotary encoder pins. For Arduino use pins 2 & 3 for Pin A and Pin B, any other GPIO for Push Button
#define PIN_A 2
#define PIN_B 3
#define PUSH_BTN 27

volatile int rotationCounter = 0;     // A detent counter for the rotary encoder (negative = anti-clockwise)
volatile bool rotaryEncoder = false;    // Flag from interrupt routine (moved=true)
volatile uint8_t PIN_B_VALUE = 0;       // Store latest PIN_B value
static int prevRotationCounter = 0;     // Track the rotation value

void rotaryA() {    // Interrupt for PIN A. For Arduino remove the IRAM_ATTR bit
  static unsigned long isrMillis = 0;   // Only allow interrupts as per datasheet: 60rpm = 1rps = 12pps = 1 pulse every 40mS
  if (millis() - isrMillis >= 20) {     // We can adjust the Real World delay-between-pulses to suit, may work with a pulse 20mS
    // Set flag
    rotaryEncoder = true;
    if (digitalRead(PIN_A)) {             // Clockwise
      if (digitalRead(PIN_B) == LOW) {
        // if (PIN_B_VALUE == 0) {
        rotationCounter++;
      } else {
        rotationCounter--;
      }
    }
    if (!digitalRead(PIN_A)) {    // Anti-clockwise
      if (digitalRead(PIN_B) == HIGH) {
        // if (PIN_B_VALUE == HIGH) {
        rotationCounter++;
      } else {
        rotationCounter--;
      }
    }
  } else {
    debugln("Too fast (ignored)");
  }
  isrMillis = millis();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_A, INPUT);    // Outputs are controlled by 74HAC14
  pinMode(PIN_B, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_A), rotaryA, CHANGE);   // We need to monitor a change on PIN A (rising and falling)
  debugln("Setup completed - turn the rotary encoder!");
}

void loop() {
  checkRotary();
}

void checkRotary() {
  if (rotaryEncoder) {    // Has rotary encoder moved?
    rotaryEncoder = false;
    debugln(rotationCounter < prevRotationCounter ? "L" : "R");
    debug(rotationCounter);
    prevRotationCounter = rotationCounter;
  }
  if (digitalRead(PUSH_BTN) == HIGH) {      // Push button?
    rotationCounter     = 0;
    prevRotationCounter = 0;
    debugln("Reset to ZERO");
//    while (digitalRead(PUSH_BTN))     // Wait until button released before continuing (blocking!)
//      ;
  }
}
