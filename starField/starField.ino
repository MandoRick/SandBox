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

#include "SH1107.h"

#define DISP_W 128
#define DISP_H 128
#define FRAME_COUNT 13
#define OLED_CLOCK_SPEED 1999999ul
#define MAX_STARS 20  // Adjust the maximum number of stars

int starCount = 0;
float shipSpeed = 2;               // Adjust as needed
bool displayArea[DISP_W][DISP_H];  // Represents the entire display area
int starCoordinates[MAX_STARS][2];
int starMovementDirection[MAX_STARS];
float starSpeed[MAX_STARS];  // Speed of each star

SH1107 display;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  display.begin();
  Wire.setClock(OLED_CLOCK_SPEED);
}

void loop() {
  calculateStars();
  moveStars();
  renderStars();
  // Render the cockpit
  renderCockpit();
  display.display();
}

void calculateStars() {
  if (starCount < MAX_STARS) {
    generateStars();
    starCount++;
  }
}

void generateStars() {
  // Generate a new star at the center of the display
  starCoordinates[starCount][0] = DISP_W / 2 + random(-5, 5);
  starCoordinates[starCount][1] = DISP_H / 2 + random(-5, 5);
  // Generate random movement directions for the new star
  starMovementDirection[starCount] = random(360);  // any heading from 0 to 359
  // Initialize star speed
  // Adjust the speed based on distance from the center for smoother movement
  float distanceFromCenter = sqrt(pow(starCoordinates[starCount][0] - DISP_W / 2, 2) + pow(starCoordinates[starCount][1] - DISP_H / 2, 2));
  float initialSpeed = 0.001;                                                             // Adjust the initial speed as needed
  starSpeed[starCount] = initialSpeed * (1.0 + tanh(distanceFromCenter / (DISP_W / 4)));  // Using hyperbolic tangent function
}

void moveStars() {
  for (int i = 0; i < starCount; i++) {
    // Calculate the distance from the center
    float distanceFromCenter = sqrt(pow(starCoordinates[i][0] - DISP_W / 2, 2) + pow(starCoordinates[i][1] - DISP_H / 2, 2));

    // Calculate the speed multiplier based on the distance from the center
    // Adjust the multiplier function as needed to achieve the desired effect
    float speedMultiplier = 1.0 + tanh(distanceFromCenter / (DISP_W / 2));  // Using hyperbolic tangent function

    // Calculate movement components based on angle and adjusted speed
    float angleRad = starMovementDirection[i] * PI / 180.0;  // Convert angle to radians
    float dx = cos(angleRad) * shipSpeed * speedMultiplier;  // X component with adjusted speed
    float dy = sin(angleRad) * shipSpeed * speedMultiplier;  // Y component with adjusted speed

    // Update star coordinates based on movement components
    starCoordinates[i][0] += dx;
    starCoordinates[i][1] += dy;

    // If the star goes beyond the display boundaries, reset its position to the center
    if (starCoordinates[i][0] >= DISP_W || starCoordinates[i][0] < 0 || starCoordinates[i][1] >= DISP_H || starCoordinates[i][1] < 0) {
      starCoordinates[i][0] = DISP_W / 2 + random(-2, 2);
      starCoordinates[i][1] = DISP_H / 2 + random(-2, 2);
      starMovementDirection[i] = random(360);  // any heading from 0 to 359
    }
  }
}

void renderStars() {
  // Clear the display area
  for (int x = 0; x < DISP_W; x++) {
    for (int y = 0; y < DISP_H; y++) {
      displayArea[x][y] = false;
    }
  }
  // Render stars
  for (int i = 0; i < starCount; i++) {
    int x = (int)starCoordinates[i][0];
    int y = (int)starCoordinates[i][1];
    if (x >= 0 && x < DISP_W && y >= 0 && y < DISP_H) {
      displayArea[x][y] = true;
    }
  }
  // Draw the display area on the screen
  for (int x = 0; x < DISP_W; x++) {
    for (int y = 0; y < DISP_H; y++) {
      display.drawPixel(x, y, displayArea[x][y] ? WHITE : BLACK);
    }
  }
}

void renderCockpit() {
  display.drawLine(0, 0, 30, 30, WHITE);
  display.drawLine(0, 127, 30, 97, WHITE);
  display.drawLine(127, 0, 97, 30, WHITE);
  display.drawLine(127, 127, 97, 97, WHITE);
  display.drawLine(30, 30, 97, 30, WHITE);
  display.drawLine(30, 97, 97, 97, WHITE);
  display.drawCircle(64, 64, 3, WHITE);
  for (int x = 15; x >= 0; x = x - 2) {
    display.drawCircle(64, 112, x, WHITE);
  }
  for (int x = 14; x >= 0; x = x - 2) {
    display.drawCircle(64, 112, x, BLACK);
  }
}