#define BUTTON_COUNT (2)

uint8_t interrupt_Pins[BUTTON_COUNT]{ 16, 17 };
volatile bool interrupt_Flag[BUTTON_COUNT]{ false, false };
unsigned long current_Time = 0;
unsigned long previous_Time = 0;
const unsigned long interval_Time = 1500;

void setup() {
  Serial.begin(115200);
  attachInterrupt(digitalPinToInterrupt(interrupt_Pins[0]), trigger0, FALLING);
  attachInterrupt(digitalPinToInterrupt(interrupt_Pins[1]), trigger1, FALLING);
}

void loop() {
  current_Time = millis();
  if ((unsigned long)(current_Time - previous_Time) >= interval_Time) {
    for (int i = 0; i < BUTTON_COUNT; i++) {
      if (interrupt_Flag[i] == true) {
        interrupt_Flag[i] = false;
        Serial.println("Button " + (String)i + " has been pressed");
      }
    }
    previous_Time = current_Time;
  }
}

void trigger0() {
  interrupt_Flag[0] = true;
}

void trigger1() {
  interrupt_Flag[1] = true;
}