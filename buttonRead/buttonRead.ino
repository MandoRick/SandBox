/*
  --------------- by JediRick -----------------
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
  --------------------------------------------
*/

#include "button.h"
#define ledPin 13
Button buttonA(6);
int count = 0;

void setup() {
  delay(2000);
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  readTheButton();
  checkLogic();
}

void readTheButton(void) {
  Serial.println(count);
  if (buttonA.pressed()) {      // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    count++;
  }
  if (count > 5) {
    count = 0;
  }
}

void checkLogic() {
  switch (count) {
    case 0:    // your hand is on the sensor
      Serial.println("case 0");
      firstCase();
      break;
    case 1:    // your hand is close to the sensor
      Serial.println("case 1");
      break;
    case 2:    // your hand is a few inches from the sensor
      Serial.println("case 2");
      break;
    case 3:    // your hand is nowhere near the sensor
      Serial.println("case 3");
      break;
    case 4:    // your hand is nowhere near the sensor
      Serial.println("case 4");
      break;
    case 5:    // your hand is nowhere near the sensor
      Serial.println("case 5");
      break;
  }
}

void firstCase(void) {
  Serial.println("first case activated");
}
