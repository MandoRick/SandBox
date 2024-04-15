/*-data-type------size---------description----------------------
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
  --------------------------------------------------------------
  camelCase                -  anything that changes
  snake_case               -  variable's that are exclusive in a function
  Snake_Case               -  CLASS/struct exclusave varables/functions
  iNVERTEDcAMELcASE        -  outside code that is being accessed [database]
  SNake_CAse               -  duplicate varables inside the case function [frequently used in library names]
  ALL_CAPS                 -  const varable names or defines
  ------------- by jediRick & RefreshMyMind --------------------
*/

#include <GyverFIFO.h>
#include <Wire.h>
#include "sh1107.h"
#include "data.h"
#include "sound.h"

#define ANALOG_PIN A0
#define BUTTON_QTY 3
#define BUTTON_PIN_1 6
#define BUTTON_PIN_2 7
#define BUTTON_PIN_3 8
#define BUTTONDEBOUNCETIME 1

#define DISP_W 128
#define DISP_H 128
#define OLED_CLOCK_SPEED 1999999ul

#define WHITE 1
#define BLACK 0

#define SCORE_DIGIT_QTY 6
#define ASTEROID_QTY 6
#define ASTEROID_DATA_PTS 4  // x, y, radius, speed

#define BUFFER_QTY 10

#define PROJECTILE_SPEED 2
#define PROJECTILE_LIFESPAN 100  // Number of frames before the projectile disappears

// Define Projectile Structure
struct Projectile {
  int x;
  int y;
  int lifespan;  // Remaining frames before the projectile disappears
  float speedX;  // Velocity component along the X-axis
  float speedY;  // Velocity component along the Y-axis
};

// Create an array to hold projectiles
#define MAX_PROJECTILES 25
Projectile projectiles[MAX_PROJECTILES];

int player_score = 0;
int player_lives = 4;
int playerScoreArray[SCORE_DIGIT_QTY];

int ship_X = DISP_W / 2;
int ship_Y = DISP_H - 20;

int asteroids_data[ASTEROID_QTY][ASTEROID_DATA_PTS];

volatile bool BUTTON_STATE[BUTTON_QTY];
volatile unsigned long lastDebounceTime[BUTTON_QTY] = { 0 };

volatile bool audioArray[BUFFER_QTY];

SH1107 display;

GyverFIFO<bool, 2> soundBuffer[BUFFER_QTY];

void setup() {
  //Serial.begin(115200);
  setupButtons();
  display.begin();
  Wire.setClock(OLED_CLOCK_SPEED);
  drawLogo();
  updatePlayerScoreArray();
  randomSeed(analogRead(A0));
  setupAsteroids();
  initProjectiles();
  delay(5000);
  clearDisplay();
  drawReady();
  display.display();
  delay(3000);
}

void loop() {
  checkButtons();
  player_score = player_score + 1;
  if (player_score > 999999) {
    player_score = 0;
  }

  updatePlayerScoreArray();
  updateProjectiles();
  drawDisplay();
  //delay(5);
}

void updatePlayerScoreArray() {
  // Calculate the number of digits in the score
  int numDigits = (int)log10(player_score) + 1;

  // Fill leading zeroes if necessary
  int leadingZeroes = SCORE_DIGIT_QTY - numDigits;
  if (leadingZeroes < 0) {
    leadingZeroes = 0;  // Ensure leadingZeroes is non-negative
  }

  // Store each digit of Score into playerScoreArray with leading zeroes
  uint32_t tempScore = player_score;
  for (int i = SCORE_DIGIT_QTY - 1; i >= leadingZeroes; i--) {
    playerScoreArray[i] = tempScore % 10;  // Extract the least significant digit
    tempScore /= 10;                       // Shift right to get next digit
  }

  // Fill leading zeroes
  for (int i = 0; i < leadingZeroes; i++) {
    playerScoreArray[i] = 0;
  }
}

void setupAsteroids() {
  for (int i = 0; i < ASTEROID_QTY; i++) {
    int x = random(DISP_W);
    asteroids_data[i][0] = x;
    int y = 0;
    asteroids_data[i][1] = y;
    int radius = random(3, 8);
    asteroids_data[i][2] = radius;
    int heading = 180;
    asteroids_data[i][3] = heading;
    int speed = random(1, 2);
    asteroids_data[i][3] = speed; // Note: Changed to [3] to stay within bounds
  }
}


void clearDisplay() {
  for (int x = 0; x < DISP_W; x++) {
    for (int y = 0; y < DISP_H; y++) {
      display.drawPixel(x, y, BLACK);
    }
  }
}

void drawLogo() {
  for (int i = 0; i < DISP_H; i++) {
    for (int j = 0; j < DISP_W; j++) {
      display.drawPixel(j, i, LOGO[i][j]);
    }
  }
  display.display();
}

void drawReady() {
  int startX = (DISP_W - 5 * 10) / 2;  // Calculate the starting X position for "READY"
  int startY = (DISP_H - 7) / 2;       // Calculate the starting Y position for "READY"
  for (int i = 0; i < 5; i++) {
    drawLetter(startX + i * 10, startY, READY[i]);
  }
}

void drawDead() {
  int startX = (DISP_W - 4 * 10) / 2;  // Calculate the starting X position for "DEAD"
  int startY = (DISP_H - 7) / 2;       // Calculate the starting Y position for "DEAD"
  for (int i = 0; i < 4; i++) {
    drawLetter(startX + i * 10, startY, DEAD[i]);
  }
}

void drawDisplay() {
  clearDisplay();
  drawScore();
  drawLives();
  drawShip();
  drawProjectiles();
  drawAsteroids();
  checkShipCollision();
  display.display();
}

void drawScore() {
  // Draw playerScoreArray onto the display
  for (int i = 0; i < SCORE_DIGIT_QTY; i++) {
    drawDigit(i * 10, 0, playerScoreArray[i]);
  }
}

void drawLives() {
  // Draw player_lives onto the display
  for (int i = 0; i < player_lives; i++) {
    int x = i * 10 + 80;
    calcLives(x, 12);
  }
}

void drawShip() {
  calcShip(ship_X, ship_Y);
}

void drawAsteroids() {
  for (int i = 0; i < ASTEROID_QTY; i++) {
    calcAsteroids(asteroids_data[i][0], asteroids_data[i][1], asteroids_data[i][3]);
    display.drawCircle(asteroids_data[i][0], asteroids_data[i][1], asteroids_data[i][2], WHITE);

    // Update the asteroid's position in the array
    asteroids_data[i][0] = constrain(asteroids_data[i][0], 0, DISP_W - 1);
    asteroids_data[i][1] = constrain(asteroids_data[i][1], 0, DISP_H - 1);
  }
}

void drawDigit(int x, int y, int digit) {
  // Segment positions relative to the top-left corner of the digit
  const int segmentX[7] PROGMEM = { 1, 1, 1, 1, 1, 5, 5 };
  const int segmentY[7] PROGMEM = { 1, 5, 9, 1, 5, 1, 5 };
  const int segmentEndX[7] PROGMEM = { 5, 5, 5, 1, 1, 5, 5 };
  const int segmentEndY[7] PROGMEM = { 1, 5, 9, 5, 9, 5, 9 };

  for (int i = 0; i < 7; i++) {
    if (pgm_read_byte(&NUMBERS[digit][i])) {
      // Draw segment i
      display.drawLine(
        x + pgm_read_byte(&segmentX[i]),     // x1
        y + pgm_read_byte(&segmentY[i]),     // y1
        x + pgm_read_byte(&segmentEndX[i]),  // x2
        y + pgm_read_byte(&segmentEndY[i]),  // y2
        WHITE);
    }
  }
}

void drawLetter(int x, int y, const bool letter[7]) {
  // Segment positions relative to the top-left corner of the letter
  const int segmentX[7] = { 1, 1, 1, 1, 1, 5, 5 };
  const int segmentY[7] = { 1, 5, 9, 1, 5, 1, 5 };
  const int segmentEndX[7] = { 5, 5, 5, 1, 1, 5, 5 };
  const int segmentEndY[7] = { 1, 5, 9, 5, 9, 5, 9 };

  for (int i = 0; i < 7; i++) {
    if (letter[i]) {
      // Draw segment i
      display.drawLine(
        x + segmentX[i],     // x1
        y + segmentY[i],     // y1
        x + segmentEndX[i],  // x2
        y + segmentEndY[i],  // y2
        WHITE);
    }
  }
}

void calcLives(int x, int y) {
  // Draw the ship icon (triangle)
  int x1 = x + 3;  // Top point
  int y1 = y - 9;
  int x2 = x;  // Bottom-left point
  int y2 = y;
  int x3 = x + 6;  // Bottom-right point
  int y3 = y;

  // Draw the lines forming the triangle
  display.drawLine(x1, y1, x2, y2, WHITE);
  display.drawLine(x1, y1, x3, y3, WHITE);
  display.drawLine(x2, y2, x3, y3, WHITE);
}

void calcShip(int x, int y) {
  // Define ship dimensions
  int shipWidth = 12;  // Width of the ship
  int shipHeight = 6;  // Height of the ship

  // Calculate the coordinates of the ship's vertices
  int x1 = x - shipWidth / 2;   // Top-left x-coordinate
  int y1 = y - shipHeight / 2;  // Top-left y-coordinate
  int x2 = x + shipWidth / 2;   // Top-right x-coordinate
  int y2 = y - shipHeight / 2;  // Top-right y-coordinate
  int x3 = x + shipWidth / 2;   // Bottom-right x-coordinate
  int y3 = y + shipHeight / 2;  // Bottom-right y-coordinate
  int x4 = x - shipWidth / 2;   // Bottom-left x-coordinate
  int y4 = y + shipHeight / 2;  // Bottom-left y-coordinate

  // Draw the ship (rectangle)
  display.drawLine(x1, y1, x2, y2, WHITE);  // Top side
  display.drawLine(x2, y2, x3, y3, WHITE);  // Right side
  display.drawLine(x3, y3, x4, y4, WHITE);  // Bottom side
  display.drawLine(x4, y4, x1, y1, WHITE);  // Left side
}


void calcAsteroids(int& x, int& y, int speed) {
  // Calculate the change in position along the y-axis (downward direction)
  y += speed;

  // Wrap around the display if the asteroid goes out of bounds
  if (y >= DISP_H) {
    y = 0;               // Reset y-coordinate to the top of the display
    x = random(DISP_W);  // Randomize x-coordinate for a new position
  }
}


void checkShipCollision() {
  // Radius of the ship (assumed to be the same for simplicity)
  int shipRadius = 6;

  // Iterate over all asteroids
  for (int i = 0; i < ASTEROID_QTY; i++) {
    int asteroidX = asteroids_data[i][0];
    int asteroidY = asteroids_data[i][1];
    int asteroidRadius = asteroids_data[i][2];

    // Calculate the distance between the ship and the asteroid
    int distanceSquared = (ship_X - asteroidX) * (ship_X - asteroidX) + (ship_Y - asteroidY) * (ship_Y - asteroidY);

    // Check if the distance is less than the sum of the radii (collision detected)
    if (distanceSquared < (shipRadius + asteroidRadius) * (shipRadius + asteroidRadius)) {
      // Collision detected, decrease lives
      player_lives--;
      soundBuffer[9].write(1);

      // Move the asteroid to a new random position
      asteroids_data[i][0] = random(DISP_W);
      asteroids_data[i][1] = random(DISP_H);
    }
  }
  if (player_lives == 0) {
    clearDisplay();
    drawDead();
    display.display();
    delay(5000);
    player_score = 0;
    player_lives = 4;
    ship_X = DISP_W / 2;
  }
}

void triggerButton1() {
  if (millis() - lastDebounceTime[0] > BUTTONDEBOUNCETIME) {
    lastDebounceTime[0] = millis();
    BUTTON_STATE[0] = digitalRead(BUTTON_PIN_1);
    if (BUTTON_STATE[0]) {
      fireProjectile();  // Fire projectile when button 1 is pressed
    }
  }
}

void triggerButton2() {
  if (millis() - lastDebounceTime[1] > BUTTONDEBOUNCETIME) {
    lastDebounceTime[1] = millis();
    BUTTON_STATE[1] = digitalRead(BUTTON_PIN_2);
  }
}

void triggerButton3() {
  if (millis() - lastDebounceTime[2] > BUTTONDEBOUNCETIME) {
    lastDebounceTime[2] = millis();
    BUTTON_STATE[2] = digitalRead(BUTTON_PIN_3);
  }
}

void setupButtons() {
  pinMode(BUTTON_PIN_1, INPUT_PULLUP);
  pinMode(BUTTON_PIN_2, INPUT_PULLUP);
  pinMode(BUTTON_PIN_3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_1), triggerButton1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_2), triggerButton2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN_3), triggerButton3, CHANGE);
}

void checkButtons() {
  if (BUTTON_STATE[0]) {
    BUTTON_STATE[0] = false;
    soundBuffer[0].write(1);
  }
  if (BUTTON_STATE[1]) {
    BUTTON_STATE[1] = false;
    soundBuffer[1].write(1);
  }
  if (BUTTON_STATE[2]) {
    BUTTON_STATE[2] = false;
    soundBuffer[2].write(1);
  }
  int sensorValue = analogRead(ANALOG_PIN);
  ship_X = map(sensorValue, 0, 1023, 0, 127);
}

// Initialize Projectile Array
void initProjectiles() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    projectiles[i].lifespan = 0;  // Set all projectiles as inactive initially
  }
}

// Update Projectile Movement
void updateProjectiles() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (projectiles[i].lifespan > 0) {
      // Move the projectile upward
      projectiles[i].x += round(projectiles[i].speedX);  // No horizontal movement
      projectiles[i].y += round(projectiles[i].speedY);

      // Decrement the lifespan
      projectiles[i].lifespan--;

      // Check for collision with asteroids or screen bounds
      checkProjectileAsteroidCollision(i);
    }
  }
}

// Fire Projectiles
void fireProjectile() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (projectiles[i].lifespan == 0) {
      // Set projectile properties
      projectiles[i].x = ship_X;  // Initial position at the ship's position
      projectiles[i].y = ship_Y;
      projectiles[i].lifespan = PROJECTILE_LIFESPAN;  // Set lifespan

      // Calculate velocity components based on a constant speed (upward direction)
      float projectileSpeed = PROJECTILE_SPEED;  // Projectile speed includes ship's velocity
      projectiles[i].speedX = 0;  // No horizontal movement for projectiles
      projectiles[i].speedY = -projectileSpeed;  // Negative value for upward movement

      break;  // Exit loop after firing one projectile
    }
  }
}

void drawProjectiles() {
  for (int i = 0; i < MAX_PROJECTILES; i++) {
    if (projectiles[i].lifespan > 0) {
      // Draw projectile if active
      display.drawPixel(projectiles[i].x, projectiles[i].y, WHITE);
    }
  }
}

void checkProjectileAsteroidCollision(int projectileIndex) {
  for (int j = 0; j < ASTEROID_QTY; j++) {
    int asteroidX = asteroids_data[j][0];
    int asteroidY = asteroids_data[j][1];
    int asteroidRadius = asteroids_data[j][2];

    // Calculate the distance between the projectile and the asteroid
    int distanceSquared = (projectiles[projectileIndex].x - asteroidX) * (projectiles[projectileIndex].x - asteroidX) + (projectiles[projectileIndex].y - asteroidY) * (projectiles[projectileIndex].y - asteroidY);

    // Check if the distance is less than the sum of the radii (collision detected)
    if (distanceSquared < asteroidRadius * asteroidRadius) {
      // Collision detected, destroy the asteroid and the projectile
      destroyAsteroid(j);
      destroyProjectile(projectileIndex);
      // Optionally, increase player score or perform any other actions
      player_score += 100;
      // Break to avoid checking collision with other asteroids
      break;
    }
  }
}

void destroyAsteroid(int asteroidIndex) {
  // Move the asteroid to a new random position
  asteroids_data[asteroidIndex][0] = random(DISP_W);
  asteroids_data[asteroidIndex][1] = random(DISP_H);
}

void destroyProjectile(int projectileIndex) {
  // Reset projectile lifespan to 0 to mark it as inactive
  projectiles[projectileIndex].lifespan = 0;
}

void updateShipPosition() {
  // Read the analog input from the potentiometer
  int potValue = analogRead(ANALOG_PIN);

  // Map the potentiometer value to the display width
  // Assuming the potentiometer range is 0-1023 and display width is DISP_W
  ship_X = map(potValue, 0, 1023, 0, DISP_W);
}

void setup1() {
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);
  delay(2000);
  playAsteroidsTune();
  delay(2000);
  for (int i = 0; i < BUFFER_QTY; i++) {
    audioArray[i] = 0;
  }
}

void loop1() {
  readBuffer();
  processAudio();
}

void readBuffer() {
  for (int i = 0; i < BUFFER_QTY; i++) {
    audioArray[i] = soundBuffer[i].read();
  }
}

void processAudio() {
  if (audioArray[0]) {
    audioArray[0] = 0;
    playShootSound();
  }
  if (audioArray[1]) {
    audioArray[1] = 0;
    playShootSound();
  }
  if (audioArray[2]) {
    audioArray[2] = 0;
    playShootSound();
  }
  if (audioArray[3]) {
    audioArray[3] = 0;
    playShootSound();
  }
  if (audioArray[9]) {
    audioArray[9] = 0;
    playCrashSound();
  }
}
