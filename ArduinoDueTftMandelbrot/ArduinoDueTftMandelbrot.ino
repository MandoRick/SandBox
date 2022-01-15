#include <Adafruit_GFX.h>    // Core graphics library
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include <TouchScreen.h>

#include "mandelbrot.h"

const int XP=8,XM=A2,YP=A3,YM=9; //240x320 ID=0x9341
const int TS_LEFT=868,TS_RT=136,TS_TOP=126,TS_BOT=883;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Calibrate values
const short TS_MINX=116;
const short TS_MINY=113;
const short TS_MAXX=849;
const short TS_MAXY=872;

extern const uint8_t baseMandelbrot[] ;

Mandelbrot mandel;

void setup(void) {

  Serial.begin(9600);
  
  tft.reset();
  tft.begin(tft.readID());  // Use 0x9341 for the SDFP5408 if readID fails
  tft.setRotation(0);       // Needed for the Mega, please changed for your choice or rotation initial
  tft.fillScreen(0);
 
  pinMode(13, OUTPUT);
    
  mandel.setParameters(-2.2, -1.2, 0.6, 1.2);
  mandel.drawPGMData(baseMandelbrot, 76800L);  
  mandel.draw();
}
   
#define MINPRESSURE 200
#define MAXPRESSURE 1000

void loop()
{
  mandel.update();


  TSPoint pt = ts.getPoint(); 

  pinMode(XM, OUTPUT); //Pins configures again for TFT control
  pinMode(YP, OUTPUT);

  if (pt.z >= MINPRESSURE && pt.z < MAXPRESSURE) {
    pt.x = map(pt.x, TS_MINX, TS_MAXX, 0, tft.width());
    pt.y = map(pt.y, TS_MINY, TS_MAXY, 0, tft.height());;
    
    tft.fillCircle(5, 5, 10, tft.color565(0,0,0));
    mandel.zoomIntoPoint(pt.x, pt.y, 10);
    mandel.draw();
  }
 
}
