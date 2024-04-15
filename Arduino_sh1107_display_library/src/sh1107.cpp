#include "SH1107.h"

void SH1107::begin(TwoWire &wireInstance) {
  this->wireInstance = &wireInstance; // Store the pointer to the Wire instance
  this->wireInstance->begin();
  sendCommand(SH110X_DISPLAYOFF);
  sendCommand(SH110X_SETDISPLAYCLOCKDIV, 0x51);
  sendCommand(SH110X_MEMORYMODE);
  sendCommand(SH110X_DISPLAYON);
}

void SH1107::drawPixel(int x, int y, uint8_t color) {
  if (x >= 0 && x <= 128 && y >= 0 && y <= 128) {
    uint8_t page = y / 8;
    uint8_t bitMask = 1 << (y % 8);

    if (color == BLACK) {
      displayBuffer[x + page * 128] &= ~bitMask;
    } else {
      displayBuffer[x + page * 128] |= bitMask;
    }
  }
}

void SH1107::drawCircle(int x0, int y0, int radius, uint8_t color) {
  int x = radius - 1;
  int y = 0;
  int dx = 1;
  int dy = 1;
  int err = dx - (radius << 1);

  while (x >= y) {
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 - y, y0 - x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 + x, y0 - y, color);

    if (err <= 0) {
      y++;
      err += dy;
      dy += 2;
    }
    if (err > 0) {
      x--;
      dx += 2;
      err += dx - (radius << 1);
    }
  }
}

void SH1107::drawLine(int x0, int y0, int x1, int y1, uint8_t color) {
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1 - y0);
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;
  int e2;

  while (true) {
    drawPixel(x0, y0, color);
    if (x0 == x1 && y0 == y1) break;
    e2 = 2 * err;
    if (e2 >= dy) {
      err += dy;
      x0 += sx;
    }
    if (e2 <= dx) {
      err += dx;
      y0 += sy;
    }
  }
}

void SH1107::display() {
  for (int page = 0; page < 16; ++page) {
    setCursor(0, page);
    beginData();
    for (int x = 0; x < 128; ++x) {
      sendData(displayBuffer[x + page * 128]);
    }
    endTransm();
  }
}

void SH1107::setCursor(uint8_t x, uint8_t y) {
  sendCommand(SH110X_SETLOWCOLUMN | (x & 0x0F));
  sendCommand(SH110X_SETHIGHCOLUMN | ((x >> 4) & 0x0F));
  sendCommand(SH110X_SETPAGEADDR | (y & 0x0F));
}

void SH1107::sendCommand(uint8_t command, uint8_t arg) {
  this->wireInstance->beginTransmission(SH1107_I2C_ADDRESS);
  this->wireInstance->write(SH1107_COMMAND_MODE);
  this->wireInstance->write(command);
  if (arg != 0x00) {
    this->wireInstance->write(arg);
  }
  this->wireInstance->endTransmission();
}

void SH1107::beginData() {
  this->wireInstance->beginTransmission(SH1107_I2C_ADDRESS);
  this->wireInstance->write(SH1107_DATA_MODE);
}

void SH1107::endTransm() {
  this->wireInstance->endTransmission();
}

void SH1107::sendData(uint8_t data) {
  this->wireInstance->write(data);
}
