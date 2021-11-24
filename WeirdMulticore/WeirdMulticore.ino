#include <Arduino.h>
#define led LED_BUILTIN
bool blinkState = false;
float num1 = 0;
float num2 = 1;
float answer;
float result;
uint32_t alpha;
uint32_t bravo;
uint32_t charlie;
uint32_t delta;
uint32_t count;
uint32_t ending;

void setup() {
  delay(4500);
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void setup1() {
  delay (5000);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  alpha = rp2040.getCycleCount();
  calc();
  bravo = rp2040.getCycleCount();
  count = bravo - alpha;
}

void loop1() {
  doPrints();
  delay(100);
}

void calc() {
  for (int i = 0; i < 200; i++) {
    for (int y = 0; y < 255; y++) {
      digitalWrite(LED_BUILTIN, y);
    }
    for (int y = 255; y > 0; y--) {
      digitalWrite(LED_BUILTIN, y);
    }
  }
  digitalWrite(LED_BUILTIN, 0);
}

void doPrints() {
  Serial.print("Core 1 cycles: ");
  Serial.println(count);
  Serial.printf("Core temperature: %2.1fC\n", analogReadTemp());
}
