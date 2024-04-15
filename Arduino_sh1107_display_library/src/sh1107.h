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

  void begin(TwoWire &wireInstance = Wire);
  void drawPixel(int x, int y, uint8_t color);
  void drawCircle(int x0, int y0, int radius, uint8_t color);
  void drawLine(int x0, int y0, int x1, int y1, uint8_t color);
  void display();

private:
  uint8_t displayBuffer[BUFSIZE_128x128];
  TwoWire* wireInstance; // Pointer to the Wire instance

  void setCursor(uint8_t x, uint8_t y);
  void sendCommand(uint8_t command, uint8_t arg = 0x00);
  void beginData();
  void endTransm();
  void sendData(uint8_t data);
};

#endif
