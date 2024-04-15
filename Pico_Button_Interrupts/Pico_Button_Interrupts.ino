#define BUTTON_COUNT 4
#define BUTTON_DEBOUNCE_MILLIS 200

uint8_t buttonPins[BUTTON_COUNT] = { 16, 17, 18, 19 };
volatile bool buttonFlags[BUTTON_COUNT] = { false, false, false, false };
unsigned long lastButtonPressTime[BUTTON_COUNT] = { 0 };

void button0Interrupt() {
    buttonInterruptHandler(0);
}

void button1Interrupt() {
    buttonInterruptHandler(1);
}

void button2Interrupt() {
    buttonInterruptHandler(2);
}

void button3Interrupt() {
    buttonInterruptHandler(3);
}

void button0Job() {
    Serial.println("Performing button 0 job");
}

void button1Job() {
    Serial.println("Performing button 1 job");
}

void button2Job() {
    Serial.println("Performing button 2 job");
}

void button3Job() {
    Serial.println("Performing button 3 job");
}

void (*buttonInterruptHandlers[BUTTON_COUNT])() = { button0Interrupt, button1Interrupt, button2Interrupt, button3Interrupt };
void (*buttonJobs[BUTTON_COUNT])() = { button0Job, button1Job, button2Job, button3Job };

void setup() {
    Serial.begin(115200);
    setupButtons();
}

void loop() {
    checkButtonFlags();
}

void setupButtons() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        attachInterrupt(digitalPinToInterrupt(buttonPins[i]), buttonInterruptHandlers[i], RISING);
    }
}

void checkButtonFlags() {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (buttonFlags[i]) {
            if (millis() - lastButtonPressTime[i] >= BUTTON_DEBOUNCE_MILLIS) {
                Serial.print("Button ");
                Serial.print(i);
                Serial.println(" pressed");
                buttonJobs[i]();
                lastButtonPressTime[i] = millis();
                buttonFlags[i] = false;
            }
        }
    }
}

void buttonInterruptHandler(int buttonIndex) {
    buttonFlags[buttonIndex] = true;
    lastButtonPressTime[buttonIndex] = millis();
}
