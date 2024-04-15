#include "sh1107.h"
#include "data.h"

#define dispWidth 128
#define dispHeight 128
#define FRAME_COUNT 13
#define OLED_CLOCK_SPEED 1999999ul

uint32_t timer;
int currentAnimationIndex = 0;

bool* animationArrays[FRAME_COUNT] = {
  (bool*)Wofl_Run_00,
  (bool*)Wofl_Run_01,
  (bool*)Wofl_Run_02,
  (bool*)Wofl_Run_03,
  (bool*)Wofl_Run_04,
  (bool*)Wofl_Run_05,
  (bool*)Wofl_Run_06,
  (bool*)Wofl_Run_07,
  (bool*)Wofl_Run_08,
  (bool*)Wofl_Run_09,
  (bool*)Wofl_Run_10,
  (bool*)Wofl_Run_11,
  (bool*)Wofl_Run_12,
};

SH1107 display;

void setup() {
  Serial.begin(115200);
  display.begin();
  Wire.setClock(OLED_CLOCK_SPEED);
  clearDisplay();
}

void loop() {
  Serial.println(1000 / timer);
  timer = millis();
  drawAnimation();
  timer = millis() - timer;
}

void clearDisplay() {
  for (int x = 0; x < dispWidth; x++) {
    for (int y = 0; y < dispHeight; y++) {
      display.drawPixel(x, y, BLACK);
    }
  }
  display.display();
}

void drawAnimation() {
  for (int i = 0; i < dispHeight; i++) {
    for (int j = 0; j < dispWidth; j++) {
      display.drawPixel(j, i, animationArrays[currentAnimationIndex][i * dispWidth + j]);
    }
  }
  display.display();
  currentAnimationIndex++;
  if (currentAnimationIndex >= FRAME_COUNT) {
    currentAnimationIndex = 0;
  }
}
