#include <Wire.h>  //i2c
#include <GyverOLED.h>

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1);
Adafruit_GPS GPS(&mySerial);

GyverOLED<SSH1106_128x64> oled;

#define GPSECHO  true

uint32_t timer = millis();

const uint8_t bitmap_32x32[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xE0, 0xF0, 0x70, 0x70, 0x30, 0x30, 0x30, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF0, 0x70, 0x30, 0x30, 0x20, 0x00, 0x00,
  0x00, 0x30, 0x78, 0xFC, 0x7F, 0x3F, 0x0F, 0x0F, 0x1F, 0x3C, 0x78, 0xF0, 0xE0, 0xC0, 0x80, 0x80, 0x80, 0x40, 0xE0, 0xF0, 0xF8, 0xFC, 0xFF, 0x7F, 0x33, 0x13, 0x1E, 0x1C, 0x1C, 0x0E, 0x07, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF9, 0xF7, 0xEF, 0x5F, 0x3F, 0x7F, 0xFE, 0xFD, 0xFB, 0xF1, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x1E, 0x33, 0x33, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00, 0x00,
};

void setup() {
  Serial.begin(115200);
  setupOled();
setupGps();
}

void loop() {
  updateGps();
  updateOled();
}

void setupOled() {
  oled.init();  // инициализация
}

void setupGps() {
    GPS.begin(9600);
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  // uncomment this line to turn on only the "minimum recommended" data
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
  // the parser doesn't care about other sentences at this time
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // 1 Hz update rate For the parsing code to work nicely and have time to sort thru the data, and print it out we don't suggest using anything higher than 1 Hz
  GPS.sendCommand(PGCMD_ANTENNA);       // Request updates on antenna status, comment out to keep quiet
  mySerial.println(PMTK_Q_RELEASE);
  delay(1000);
  mySerial.println(PMTK_Q_RELEASE);     // Ask for firmware version
}

void updateGps() {
    char c = GPS.read();
  // if you want to debug, this is a good time to do it!
  if ((c) && (GPSECHO))
    Serial.write(c);

  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //Serial.println(GPS.lastNMEA());   // this also sets the newNMEAreceived() flag to false

    if (!GPS.parse(GPS.lastNMEA()))   // this also sets the newNMEAreceived() flag to false
      return;  // we can fail to parse a sentence in which case we should just wait for another
  }

  // approximately every 2 seconds or so, print out the current stats
  if (millis() - timer > 2000) {
    timer = millis(); // reset the timer

    Serial.print("\nTime: ");
    if (GPS.hour < 10) { Serial.print('0'); }
    Serial.print(GPS.hour, DEC); Serial.print(':');
    if (GPS.minute < 10) { Serial.print('0'); }
    Serial.print(GPS.minute, DEC); Serial.print(':');
    if (GPS.seconds < 10) { Serial.print('0'); }
    Serial.print(GPS.seconds, DEC); Serial.print('.');
    if (GPS.milliseconds < 10) {
      Serial.print("00");
    } else if (GPS.milliseconds > 9 && GPS.milliseconds < 100) {
      Serial.print("0");
    }
    Serial.println(GPS.milliseconds);
    Serial.print("Date: ");
    Serial.print(GPS.day, DEC); Serial.print('/');
    Serial.print(GPS.month, DEC); Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("Fix: "); Serial.print((int)GPS.fix);
    Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
    if (GPS.fix) {
      Serial.print("Location: ");
      Serial.print(GPS.latitude, 4); Serial.print(GPS.lat);
      Serial.print(", ");
      Serial.print(GPS.longitude, 4); Serial.println(GPS.lon);

      Serial.print("Speed (knots): "); Serial.println(GPS.speed);
      Serial.print("Angle: "); Serial.println(GPS.angle);
      Serial.print("Altitude: "); Serial.println(GPS.altitude);
      Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
      Serial.print("Antenna status: "); Serial.println((int)GPS.antenna);
    }
  }
}

void updateOled() {
  oled.clear();   // очистить дисплей (или буфер)
  oled.update();  // обновить. Только для режима с буфером! OLED_BUFFER

  // --------------------------
  oled.home();            // курсор в 0,0
  oled.print("Hello!");   // печатай что угодно: числа, строки, float, как Serial!
  oled.update();
  delay(2000);

  // --------------------------
  oled.setCursor(5, 1);   // курсор в (пиксель X, строка Y)
  oled.setScale(2);
  oled.print("Hello!");
  oled.update();
  delay(2000);

  // --------------------------
  oled.setCursorXY(15, 30); // курсор в (пиксель X, пиксель Y)
  oled.setScale(3);
  oled.invertText(true);    // инвертируй текст!
  oled.print("Привет!");
  oled.update();
  delay(2000);

  // --------------------------
  oled.clear();
  oled.home();
  oled.setScale(1);
  oled.invertText(false);
  oled.autoPrintln(true);   // автоматически переносить текст
  oled.print(F("Lorem ipsum dolor sit amet, лорем ипсум долор сит амет привет народ ё, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam"));
  oled.update();
  delay(2000);

  // --------------------------
  oled.home();
  oled.textMode(BUF_ADD);
  // BUF_ADD - наложить текст
  // BUF_SUBTRACT - вычесть текст
  // BUF_REPLACE - заменить (весь прямоугольник буквы)
  oled.home();
  oled.setScale(3);
  oled.print("KEK!");
  oled.update();
  delay(2000);

  // --------------------------
  // СЕРВИС
  //oled.setContrast(10);   // яркость 0..255
  //oled.setPower(true);    // true/false - включить/выключить дисплей
  //oled.flipH(true);       // true/false - отзеркалить по горизонтали
  //oled.flipV(true);       // true/false - отзеркалить по вертикали
  //oled.isEnd();           // возвращает true, если дисплей "кончился" - при побуквенном выводе

  // --------------------------
  oled.clear();
  oled.dot(0, 0);     // точка на x,y
  oled.dot(0, 1, 1);  // третий аргумент: 0 выкл пиксель, 1 вкл пиксель (по умолч)
  oled.line(5, 5, 10, 10);        // линия x0,y0,x1,y1
  //oled.line(5, 5, 10, 10, 0);   // пятый аргумент: 0 стереть, 1 нарисовать (по умолч)
  oled.fastLineH(0, 5, 10);       // горизонтальная линия (y, x1, x2)
  //oled.fastLineH(0, 5, 10, 0);  // четвёртый аргумент: 0 стереть, 1 нарисовать (по умолч)
  oled.fastLineV(0, 5, 10);       // аналогично верт. линия (x, y1, y2)
  oled.rect(20, 20, 30, 25);      // прямоугольник (x0,y0,x1,y1)
  oled.rect(5, 35, 35, 60, OLED_STROKE);      // прямоугольник (x0,y0,x1,y1)
  // параметры фигуры:
  // OLED_CLEAR - очистить
  // OLED_FILL - залить
  // OLED_STROKE - нарисовать рамку
  oled.roundRect(50, 5, 80, 25, OLED_STROKE);  // аналогично скруглённый прямоугольник
  oled.circle(60, 45, 15, OLED_STROKE);        // окружность с центром в (x,y, с радиусом)
  oled.circle(60, 45, 5, OLED_FILL);           // четвёртый аргумент: параметр фигуры

  // битмап
  oled.drawBitmap(90, 16, bitmap_32x32, 32, 32, BITMAP_NORMAL, BUF_ADD);
  //oled.drawBitmap(90, 16, bitmap_32x32, 32, 32);  // по умолч. нормал и BUF_ADD
  // x, y, имя, ширина, высота, BITMAP_NORMAL(0)/BITMAP_INVERT(1), BUF_ADD/BUF_SUBTRACT/BUF_REPLACE

  oled.update();
}
