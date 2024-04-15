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

#define DISP_W 128
#define DISP_H 128
#define GAMEBOARD_W 10
#define GAMEBOARD_H 20
#define OLED_CLOCK_SPEED 1999999ul

#define SCORE_DIGIT_QTY 6

bool gameBoard[GAMEBOARD_H][GAMEBOARD_W] PROGMEM = {
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 0, 0, 1, 1, 1, 0, 0, 0, 1 },
  { 1, 0, 1, 0, 1, 1, 1, 0, 1, 1 },
  { 1, 0, 0, 1, 1, 1, 1, 0, 1, 1 },
  { 1, 0, 1, 0, 1, 1, 1, 0, 1, 1 },
  { 1, 0, 1, 0, 1, 1, 1, 0, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
};

enum TetriminoType {
  STRAIGHT,
  SQUARE,
  T_SHAPE,
  L_SHAPE1,
  L_SHAPE2,
  S_SHAPE1,
  S_SHAPE2,
  NUM_TETRIMINO_TYPES
};

bool tetriminos[NUM_TETRIMINO_TYPES][4][4] = {
  // STRAIGHT
  {
    { 0, 0, 0, 0 },
    { 1, 1, 1, 1 },
    { 0, 0, 0, 0 },
    { 0, 0, 0, 0 } },
  // SQUARE
  {
    { 1, 1 },
    { 1, 1 },
    { 0, 0 },
    { 0, 0 } },
  // T_SHAPE
  {
    { 0, 1, 0 },
    { 1, 1, 1 },
    { 0, 0, 0 } },
  // L_SHAPE1
  {
    { 1, 0 },
    { 1, 0 },
    { 1, 1 } },
  // L_SHAPE2
  {
    { 0, 1 },
    { 0, 1 },
    { 1, 1 } },
  // S_SHAPE1
  {
    { 0, 1, 1 },
    { 1, 1, 0 },
    { 0, 0, 0 } },
  // S_SHAPE2
  {
    { 1, 1, 0 },
    { 0, 1, 1 },
    { 0, 0, 0 } }
};

TetriminoType currentTetrimino = NUM_TETRIMINO_TYPES;
int tetriminoX = 0;
int tetriminoY = 0;

int player_score = 0;
int playerScoreArray[SCORE_DIGIT_QTY];

SH1107 oled;

void setup() {
  randomSeed(analogRead(A0));
  oled.begin();
  Wire.setClock(OLED_CLOCK_SPEED);
  clearDisplay();
  drawLogo();
  delay(5000);
  layoutGameBoard();
  renderGameBoard();
  delay(2000);
  clearGameBoard();
}

void loop() {
  player_score = player_score + 1;
  if (currentTetrimino == NUM_TETRIMINO_TYPES) {
    generateTetrimino();
  } else {
    moveTetriminoDown();
  }
  renderGameBoard();
  updatePlayerScoreArray();
  drawScore();
  delay(500);
}

void drawLogo() {
  for (int i = 0; i < DISP_H; i++) {
    for (int j = 0; j < DISP_W; j++) {
      oled.drawPixel(j, i, LOGO[i][j]);
    }
  }
  oled.display();
}

void layoutGameBoard() {
  clearDisplay();
  oled.drawLine(37, 25, 89, 25, WHITE);
  oled.drawLine(37, 25, 37, 127, WHITE);
  oled.drawLine(89, 25, 89, 127, WHITE);
  oled.drawLine(37, 127, 89, 127, WHITE);
  oled.display();
}

void renderGameBoard() {
  //clearDisplay();
  for (int x = 0; x < GAMEBOARD_W; x++) {
    for (int y = 0; y < GAMEBOARD_H; y++) {
      int a = 39;
      int b = 27;
      a = a + (x * 5);
      b = b + (y * 5);
      if (gameBoard[y][x]) {
        drawSquare(a, b);
      } else {
        clearSquare(a, b);
      }
    }
  }
  oled.display();
}

void clearGameBoard() {
  for (int x = 0; x < GAMEBOARD_W; x++) {
    for (int y = 0; y < GAMEBOARD_H; y++) {
      gameBoard[y][x] = 0;
    }
  }
  oled.display();
}

void clearDisplay() {
  for (int x = 0; x < DISP_W; x++) {
    for (int y = 0; y < DISP_H; y++) {
      oled.drawPixel(x, y, BLACK);
    }
  }
}

void drawSquare(int x, int y) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      oled.drawPixel(x + i, y + j, WHITE);
    }
  }
}

void clearSquare(int x, int y) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      oled.drawPixel(x + i, y + j, BLACK);
    }
  }
}

TetriminoType getRandomTetrimino() {
  return static_cast<TetriminoType>(random(NUM_TETRIMINO_TYPES));
}

void generateTetrimino() {
  // Check if there is already a Tetrimino present
  if (currentTetrimino != NUM_TETRIMINO_TYPES) {
    return;  // Exit if a Tetrimino is already present
  }

  // Generate a new Tetrimino
  TetriminoType randomTetrimino = getRandomTetrimino();
  currentTetrimino = randomTetrimino;

  // Render the Tetrimino shape on the game board starting from a random position
  tetriminoX = random(GAMEBOARD_W - 4);  // Adjust 4 according to the Tetrimino size
  tetriminoY = 0;                        // Start rendering from the top of the game board

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 4; col++) {
      if (tetriminos[currentTetrimino][row][col]) {
        gameBoard[tetriminoY + row][tetriminoX + col] = 1;
      }
    }
  }
}


// Function to move the Tetrimino down
void moveTetriminoDown() {
  // Clear the current Tetrimino's position
  clearTetrimino();

  // Check if the Tetrimino can move down
  if (canMoveTetriminoDown()) {
    tetriminoY++;  // Move the Tetrimino down by one row
  } else {
    // If the Tetrimino cannot move down, update the game board and generate a new Tetrimino
    updateGameBoard();
    tetriminoX = 0;
    tetriminoY = 0;
    currentTetrimino = NUM_TETRIMINO_TYPES;
    generateTetrimino();  // Generate a new Tetrimino
    return;
  }

  // Update the game board with the Tetrimino's new position
  updateGameBoard();
}

// Function to update the game board with the Tetrimino's shape at its current position
void updateGameBoard() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetriminos[currentTetrimino][i][j]) {
        gameBoard[tetriminoY + i][tetriminoX + j] = 1;
      }
    }
  }
}

// Function to clear the current Tetrimino's position from the game board
void clearTetrimino() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetriminos[currentTetrimino][i][j]) {
        gameBoard[tetriminoY + i][tetriminoX + j] = 0;
      }
    }
  }
}

// Function to check if the Tetrimino can move down
bool canMoveTetriminoDown() {
  // Check if the Tetrimino reaches the bottom or collides with another block
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (tetriminos[currentTetrimino][i][j]) {
        if (tetriminoY + i + 1 >= GAMEBOARD_H || gameBoard[tetriminoY + i + 1][tetriminoX + j]) {
          return false;
        }
      }
    }
  }
  return true;
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

void drawDigit(int x, int y, int digit) {
  // Segment positions relative to the top-left corner of the digit
  const int segmentX[7] PROGMEM = { 1, 1, 1, 1, 1, 5, 5 };
  const int segmentY[7] PROGMEM = { 1, 5, 9, 1, 5, 1, 5 };
  const int segmentEndX[7] PROGMEM = { 5, 5, 5, 1, 1, 5, 5 };
  const int segmentEndY[7] PROGMEM = { 1, 5, 9, 5, 9, 5, 9 };

  for (int i = 0; i < 7; i++) {
    if (pgm_read_byte(&NUMBERS[digit][i])) {
      // Draw segment i
      oled.drawLine(
        x + pgm_read_byte(&segmentX[i]),     // x1
        y + pgm_read_byte(&segmentY[i]),     // y1
        x + pgm_read_byte(&segmentEndX[i]),  // x2
        y + pgm_read_byte(&segmentEndY[i]),  // y2
        WHITE);
    }
  }
}

void drawScore() {
  for (int i = 0; i < 10; i++) {
    oled.drawLine(0, i, 55, i, BLACK);
  }
  // Draw playerScoreArray onto the display
  for (int i = 0; i < SCORE_DIGIT_QTY; i++) {
    drawDigit(i * 10, 0, playerScoreArray[i]);
  }
}