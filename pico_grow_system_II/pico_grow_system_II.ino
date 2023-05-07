/* data-type        size        description
   -------------------------------------------------------------
   boolean          8 bit       true/false
   byte             8 bit       0-255 unsigned number
   char             8 bit       -128 to 127 signed number
   unsigned char    8 bit       0-255 unsigned number
   word             16 bit      0-65535 unsigned number
   unsigned int     16 bit      0-65535 unsigned number
   int              16 bit      -32768 to 32767 signed number
   unsigned long    32 bit      0-4,294,967,295 unsigned number (usually for millis)
   long             32 bit      -2,147,483,648 to 2,147,483,647 signed number
   float            32 bit      -3.4028235E38 to 3.4028235E38 signed number
   uint8_t          8 bit       0-255 unsigned number
   int8_t           8 bit       -127 to 127 signed number
   uint16_t         16 bit      0-65,535 unsigned number
   int16_t          16 bit      -32,768 to 32,767 signed number
   uint32_t         32 bit      0-4,294,967,295 unsigned number
   int32_t          32 bit      -2,147,483,648 to 2,147,483,647 signed number
   uint64_t         64 bit      0-18,446,744,073,709,551,615 unsigned number
   int64_t          64 bit      -9,223,372,036,854,775,808 to 9,223,372,036,854,775,807 signed number
   -------------------------------------------------------------
   camelCase                   anything that changes
   snake_case                  variable names that are exclusive in a function
   Snake_Case                  class/struct exclusive variables/functions
   iNVERTEDcAMELcASE           outside code that is being accessed (e.g. database)
   SNake_CAse                  duplicate variables inside the case function (frequently used in library names)
   ALL_CAPS                    constant variable names or defines
   -------------------------------------------------------------
   by jediRick & RefreshMyMind
*/

#include <RTClib.h>
#include <GyverOLED.h>

#define relayOff (HIGH)
#define relayOn (LOW)
#define RELAY_PIN_COUNT (4)

#define RELAY_ON_TIME  5
#define RELAY_OFF_TIME 17

uint8_t RELAY_PINS[RELAY_PIN_COUNT] = { 13, 12, 11, 10 };          //light1, light2, fan1, fan2
bool relayStates[4] = { relayOff, relayOff, relayOff, relayOff };  //relay1, relay2, relay3, relay4
uint8_t timeVarH[2] = { 0, 0 };                                    //currHour, prevHour
uint8_t timeVarM[2] = { 0, 0 };                                    //currMin, prevMin
uint8_t timeVarS[2] = { 0, 0 };                                    //currSec, prevSec
uint16_t dateVar[3] = { 0, 0, 0 };                                 //day, month, year
uint8_t currTemp[2] = { 0, 0 };

RTC_DS3231 rtc;
GyverOLED<SSH1106_128x64> oled;

void setup() {
  delay(5000);
  setupPins();
  delay(5000);
  setupOled();
  delay(5000);
  setupRtc();
  delay(5000);
}

void loop() {
  grabTime();
  delay(100);
  setRelay();
  delay(100);
  updateDisp();
  delay(100);
}

void setupPins() {
  for (int i = 0; i < RELAY_PIN_COUNT; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    digitalWrite(RELAY_PINS[i], relayOff);
    delay(10);
  }
}

void setupOled() {
  oled.init();
  oled.clear();
  oled.update();
  oled.setCursor(0, 0);
  oled.setScale(1);
}

void setupRtc() {
  if (!rtc.begin()) {
    oled.println("Couldn't find RTC");
    delay(5000);
  }
  //rtc.adjust(DateTime(2023, 4, 30, 14, 21, 20));
}

void grabTime() {
  DateTime now = rtc.now();
  timeVarH[0] = now.hour();
  timeVarM[0] = now.minute();
  timeVarS[0] = now.second();
  dateVar[0] = now.day();
  dateVar[1] = now.month();
  dateVar[2] = now.year();
  currTemp[0] = rtc.getTemperature();
  currTemp[1] = analogReadTemp();
}

void setRelay() {
  if (timeVarH[0] != timeVarH[1] && timeVarH[0] == RELAY_ON_TIME) {
    for (int i = 0; i < RELAY_PIN_COUNT; i++) {
      digitalWrite(RELAY_PINS[i], relayOn);
      relayStates[i] = relayOn;
    }
  }
  if (timeVarH[0] != timeVarH[1] && timeVarH[0] == RELAY_OFF_TIME) {
    for (int i = 0; i < RELAY_PIN_COUNT; i++) {
      digitalWrite(RELAY_PINS[i], relayOff);
      relayStates[i] = relayOff;
    }
  }
  timeVarH[1] = timeVarH[0];
}

void updateDisp() {
  if (timeVarS[1] != timeVarS[0]) {
    oled.clear();
    oled.setCursor(0, 0);
    oled.setScale(1);
    if (dateVar[0] < 10) {
      oled.print('0');
    }
    oled.print(dateVar[0]);
    oled.print('/');
    if (dateVar[1] < 10) {
      oled.print('0');
    }
    oled.print(dateVar[1]);
    oled.print('/');
    oled.print(dateVar[2]);
    oled.println("");
    oled.println("");
    oled.setScale(2);
    if (timeVarH[0] < 10) {
      oled.print('0');
    }
    oled.print(timeVarH[0]);
    oled.print(':');
    if (timeVarM[0] < 10) {
      oled.print('0');
    }
    oled.print(timeVarM[0]);
    oled.print(':');
    if (timeVarS[0] < 10) {
      oled.print('0');
    }
    oled.print(timeVarS[0]);
    oled.setScale(1);
    oled.println("");
    oled.println("");
    oled.println("");
    oled.print("RTC: ");
    oled.print(currTemp[0]);
    oled.print(" C");
    oled.print("   MCU: ");
    oled.print(currTemp[1]);
    oled.println(" C");
    oled.println("");
    if (relayStates[0] == relayOff) {
      oled.print("Off");
    } else {
      oled.print("On");
    }
    oled.print(" : ");
    if (relayStates[1] == relayOff) {
      oled.print("Off");
    } else {
      oled.print("On");
    }
    oled.print(" : ");
    if (relayStates[2] == relayOff) {
      oled.print("Off");
    } else {
      oled.print("On");
    }
    oled.print(" : ");
    if (relayStates[3] == relayOff) {
      oled.print("Off");
    } else {
      oled.print("On");
    }
    oled.update();
    timeVarS[1] = timeVarS[0];
  }
}
