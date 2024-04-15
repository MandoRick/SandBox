#ifndef _SH1107_H_
#define _SH1107_H_

#include <Wire.h>

#define SH1107_I2C_ADDRESS 0x3C
#define BLACK 0
#define WHITE 1

#define SH1107_COMMAND_MODE 0x00
#define SH1107_DATA_MODE 0x40

#define SH110X_DISPLAYON 0xAF
#define SH110X_DISPLAYOFF 0xAE
#define SH110X_SETDISPLAYCLOCKDIV 0xD5
#define SH110X_MEMORYMODE 0x20
#define SH110X_SETLOWCOLUMN 0x00
#define SH110X_SETHIGHCOLUMN 0x10
#define SH110X_SETSTARTLINE 0x40
#define SH110X_SETPAGEADDR 0xB0

#define BUFSIZE_128x128 (128 * 128 / 8)

class SH1107 {
public:
  SH1107() {}

  void begin() {
    Wire.begin();
    sendCommand(SH110X_DISPLAYOFF);
    sendCommand(SH110X_SETDISPLAYCLOCKDIV, 0x51);
    sendCommand(SH110X_MEMORYMODE);
    sendCommand(SH110X_DISPLAYON);
  }

  // Draw a pixel at the specified coordinates with the specified color
  void drawPixel(int x, int y, uint8_t color) {
    if (x >= 0 && x < 128 && y >= 0 && y < 128) {
      // Calculate the page and bit mask for the pixel
      uint8_t page = y / 8;
      uint8_t bitMask = 1 << (y % 8);

      // Update the display buffer based on the color
      if (color == BLACK) {
        displayBuffer[x + page * 128] &= ~bitMask; // Clear the bit for BLACK
      } else {
        displayBuffer[x + page * 128] |= bitMask; // Set the bit for WHITE
      }
    }
  }

  // Update the display with the current display buffer
  void display() {
    // Set the page address and data mode for transmission
    for (int page = 0; page < 16; ++page) {
      setCursor(0, page); // Set the cursor to the beginning of the page
      beginData(); // Begin data transmission
      // Send the display buffer data for the current page
      for (int x = 0; x < 128; ++x) {
        sendData(displayBuffer[x + page * 128]);
      }
      endTransm(); // End data transmission for the current page
    }
  }

private:
  uint8_t displayBuffer[128 * 8]; // Display buffer to hold pixel values

  // Set the cursor to the specified position
  void setCursor(uint8_t x, uint8_t y) {
    sendCommand(SH110X_SETLOWCOLUMN | (x & 0x0F));
    sendCommand(SH110X_SETHIGHCOLUMN | ((x >> 4) & 0x0F));
    sendCommand(SH110X_SETPAGEADDR | (y & 0x0F));
  }

  // Send a command to the display
  void sendCommand(uint8_t command, uint8_t arg = 0x00) {
    Wire.beginTransmission(SH1107_I2C_ADDRESS);
    Wire.write(SH1107_COMMAND_MODE);
    Wire.write(command);
    if (arg != 0x00) {
      Wire.write(arg);
    }
    Wire.endTransmission();
  }

  // Begin data transmission to the display
  void beginData() {
    Wire.beginTransmission(SH1107_I2C_ADDRESS);
    Wire.write(SH1107_DATA_MODE);
  }

  // End data transmission to the display
  void endTransm() {
    Wire.endTransmission();
  }

  // Send data to the display
  void sendData(uint8_t data) {
    Wire.write(data);
  }
};

#endif
