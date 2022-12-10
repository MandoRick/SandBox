/***************************************************
//Web: http://www.buydisplay.com
EastRising Technology Co.,LTD
****************************************************/

#ifndef EPDIF_H
#define EPDIF_H

#include <arduino.h>

// Pin definition
// GP6 SCK SDA
// GP4 DIN MOSI SLC
#define RST_PIN (10)  // Reset
#define DC_PIN (8)  // Data Command
#define CS_PIN (9)  // Chip Select
#define BUSY_PIN (11) // Busy

class EpdIf {
public:
    EpdIf(void);
    ~EpdIf(void);

    static int  IfInit(void);
    static void DigitalWrite(int pin, int value); 
    static int  DigitalRead(int pin);
    static void DelayMs(unsigned int delaytime);
    static void SpiTransfer(unsigned char data);
};

#endif
