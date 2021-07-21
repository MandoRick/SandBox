float f1 = 0;
float f2 = 1;
float answer1;
float f3 = 0;
float f4 = 1;
float answer2;

void setup() {
  delay(1000);
  Serial.begin(9600);
}

void setup1() {
  delay (2000);
}

void loop() {
  delay(2000);
  uint32_t a = rp2040.getCycleCount();
  pi1();
  uint32_t b = rp2040.getCycleCount();
  Serial.print("Core 1 cycles: ");
  Serial.println(b - a);
  Serial.printf("Core temperature: %2.1fC\n", analogReadTemp());
  delay(2000);
  resetDigits1();
}

void loop1() {
  delay(2000);
  uint32_t c = rp2040.getCycleCount();
  pi2();
  uint32_t d = rp2040.getCycleCount();
  Serial.print("Core 2 cycles: ");
  Serial.println(d - c);
  Serial.printf("Core temperature: %2.1fC\n", analogReadTemp());
  delay(2000);
  resetDigits2();
}

void pi1() {
  for (int x = 1; x < 40; x++) {
    answer1 = f1 + f2;
    Serial.println(answer1);
    f2 = f1;
    f1 = answer1;
  }
}

void pi2() {
  for (int y = 1; y < 40; y++) {
    answer2 = f3 + f4;
    Serial.println(answer2);
    f4 = f3;
    f3 = answer2;
  }
}

void resetDigits1() {
  f1 = 0;
  f2 = 1;
  answer1 = 0;
}

void resetDigits2() {
  f3 = 0;
  f4 = 1;
  answer2 = 0;
}
