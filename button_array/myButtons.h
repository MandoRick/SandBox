#include "Arduino.h"
#define BUTTON_COUNT (3)     // Choose button QTY
#define DEBOUNCE_TIME (1000)  // Choose debounce time

const uint8_t BUTTON_PIN[BUTTON_COUNT] = { 16, 17, 18 };  // Add button pins as needed
volatile bool buttonFlag[BUTTON_COUNT] = {};
volatile bool jobFlag[BUTTON_COUNT] = {};
unsigned long currentMillis[BUTTON_COUNT] = {};
unsigned long previousMillis[BUTTON_COUNT] = {};

void setupButtons() {
  for (int buttonPin = 0; buttonPin < BUTTON_COUNT; buttonPin++) {
    pinMode(BUTTON_PIN[buttonPin], INPUT);
    buttonFlag[buttonPin] = false;
    jobFlag[buttonPin] = false;
    currentMillis[buttonPin] = 0;
    previousMillis[buttonPin] = 0;
  }
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