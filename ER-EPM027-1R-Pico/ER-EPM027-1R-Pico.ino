/*Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
Examples for ER-EPM027-1R
Display is Hardward SPI 4-Wire SPI Interface 
Tested and worked with:
Works with Arduino 1.6.0 IDE  
Works with Arduino DUE,Arduino mega2560,Arduino UNO Board
****************************************************/

#include <SPI.h>
#include <Wire.h>
#include "ER-EPM027-1.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(5000);
  Wire.begin();
  delay(5000);
  Epd epd;
  delay(5000);

  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed!");
    return;
  } else {
    Serial.print("e-Paper init!");
  }

  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to 
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
  #if 0  
  unsigned char image[1024];
  Paint paint(image, 176, 24);    //width should be the multiple of 8 

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "e-Paper Demo    ", &Font16, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 24, 5, paint.GetWidth(), paint.GetHeight());
  
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 5, "EastRising", &Font20, COLORED);
  epd.TransmitPartialRed(paint.GetImage(), 16, 23, paint.GetWidth(), paint.GetHeight());
  
  paint.Clear(COLORED);
  paint.DrawStringAt(8, 2, "buydisplay.com", &Font16, UNCOLORED);
  epd.TransmitPartialRed(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());
  
  paint.SetWidth(64);
  paint.SetHeight(64);

  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 10, 130, paint.GetWidth(), paint.GetHeight());
  
  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 90, 120, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  epd.TransmitPartialRed(paint.GetImage(), 10, 200, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.TransmitPartialRed(paint.GetImage(), 90, 190, paint.GetWidth(), paint.GetHeight());

  /* This displays the data from the SRAM in e-Paper module */
  epd.DisplayFrame();  
  delay(2000);
 #endif   
  
#if 1  
  /* This displays an image */
  epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);
   delay(2000); 
#endif    
#if 1   
  epd.DisplayFrame(IMAGE_BLACK1, IMAGE_RED1);  
  delay(2000);
  /* Deep sleep */
 #endif   


 delay(15000);
  epd.ClearFrame();
  epd.DisplayFrame(); 
 
  epd.Sleep();
}

void loop() {
  // put your main code here, to run repeatedly:

}
