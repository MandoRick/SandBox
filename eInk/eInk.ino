/*Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
Examples for ER-EPM027-1B
Display is Hardward SPI 4-Wire SPI Interface 
Tested and worked with:
Works with Arduino 1.6.0 IDE  
Works with Arduino DUE,Arduino mega2560,Arduino UNO Board
****************************************************/

#include <Wire.h>
#include <SPI.h>
#include "ER-EPM027-1B.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED 1
#define UNCOLORED 0

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  Epd epd;
  delay(2000);
  if (epd.Init() != 0) {
    Serial.println("e-Paper init failed");
    return;
  } else if (epd.Init() != 1) {
    Serial.println("e-Paper init success");
  }

  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to 
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
  unsigned char image[1024];
  Paint paint(image, 176, 24);  //width should be the multiple of 8

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "e-Paper Demo", &Font16, COLORED);
  epd.TransmitPartialData(paint.GetImage(), 16, 32, paint.GetWidth(), paint.GetHeight());

  paint.Clear(COLORED);
  paint.DrawStringAt(2, 2, "buydisplay.com", &Font16, UNCOLORED);
  epd.TransmitPartialData(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);
  paint.SetHeight(64);

  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
  epd.TransmitPartialData(paint.GetImage(), 10, 130, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.TransmitPartialData(paint.GetImage(), 90, 120, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  epd.TransmitPartialData(paint.GetImage(), 10, 200, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.TransmitPartialData(paint.GetImage(), 90, 190, paint.GetWidth(), paint.GetHeight());

  /* This displays the data from the SRAM in e-Paper module */
  epd.DisplayFrame();

  delay(2000);
  /* This displays an image */
  epd.DisplayFrame(IMAGE_DATA);
  delay(2000);

  epd.SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
  epd.SendData(0x87);
  epd.DisplayFrame(IMAGE_DATA);
  delay(2000);

  epd.SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
  epd.SendData(0xF7);
  epd.DisplayFrame(IMAGE_DATA1);
  delay(2000);

  epd.SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
  epd.SendData(0x87);
  epd.DisplayFrame(IMAGE_DATA1);


  //delay(15000);
  //epd.ClearFrame();
  // epd.DisplayFrame();
  /* Deep sleep */
  epd.Sleep();
}

void loop() {
  // put your main code here, to run repeatedly:
}
