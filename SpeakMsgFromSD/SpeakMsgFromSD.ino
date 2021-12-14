#include <SoftwareSerial.h>  // Needed by the EMIC2 library
#include <SD.h>  // Needed by the EMIC2 library
#include "EMIC2.h"
#define RX_PIN 1  // Connect SOUT pin of the Emic 2 module to the RX pin
#define TX_PIN 0  // Connect SIN pin of the Emic 2 module to the TX pin
#define CS_PIN 10
EMIC2 emic;  // Creates an instance of the EMIC2 library

void setup() {
  emic.begin(RX_PIN, TX_PIN, CS_PIN);
  emic.setDefaultSettings();
  emic -= 5;
}

void loop() {
  emic.ready();
  emic.speak("0001.txt", SD_C);
  emic.ready();
  emic.speak("0002.txt", SD_C);
  emic.ready();
  emic.speak("0003.txt", SD_C);
  emic.ready();
  emic.speak("0004.txt", SD_C);
  emic.ready();
  emic.speak("0005.txt", SD_C);
  emic.ready();
  emic.speak("0006.txt", SD_C);
  emic.ready();
  emic.speak("0007.txt", SD_C);
  emic.ready();
  emic.speak("0008.txt", SD_C);
  emic.ready();
  emic.speak("0009.txt", SD_C);
  emic.ready();
  emic.speak("0010.txt", SD_C);
  delay(60000);
}
