#define DAC1 25

void setup() {
  Serial.begin(115200);
}

void loop() { // Generate a Sine wave
  //int Value = 255; //255= 3.3V 128=1.65V
  //dacWrite(DAC1, Value);
  //delay(1000);
  for (int Value = 0 ; Value <= 255; Value += 5) {
    dacWrite(DAC1, Value);
    delay(5);
  }
  for (int Value = 255 ; Value >= 0; Value -= 5) {
    dacWrite(DAC1, Value);
    delay(5);
  }
}
