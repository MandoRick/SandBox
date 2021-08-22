/* -------------------
   created by JediRick
   -------------------
   boolean (8 bit) true/false
   byte (8 bit) - unsigned number from 0-255
   char (8 bit) - signed number from -128 to 127
   unsigned char (8 bit) - same as byte
   word (16 bit) - unsigned number from 0-65535
   unsigned int (16 bit)- the same as word
   int (16 bit) - signed number from -32768 to 32767
   unsigned long (32 bit) - unsigned number from 0-4,294,967,295  Usually for millis
   long (32 bit) - signed number from -2,147,483,648 to 2,147,483,647
   float (32 bit) - signed number from -3.4028235E38 to 3.4028235E38  Floating point not native so avoid if you can
*/

#include "data.h"

const String p_project = "Lottery Evaluator";
const uint8_t version_hi = 0;
const uint8_t version_lo = 3;
const int arrayDigits = 14336;
long array2[arrayDigits];


void versionPrint(void)
{
  Serial.print("RicksWorx: ");
  Serial.println(p_project);
  Serial.print("Version: ");
  Serial.print(version_hi);
  Serial.print('.');
  Serial.println(version_lo);
}


void setup()
{
  delay(5000);
  Serial.begin(115200);
  versionPrint();
}

void loop()
{
  //evalute array 1 and write to array 2
  for (int i = 1; i < arrayDigits; i++) {
    array2[array1[i]]++;
  }

  //print array 1
  for (int j = 1; j < arrayDigits; j++)
  {
    Serial.print(array1[j]);
    Serial.print(" ");
  }
  Serial.println(" ");

  //print array2
  for (int k = 1; k < 60; k++)
  {
    Serial.print("number ");
    Serial.print(k);
    Serial.print(": ");
    Serial.print(array2[k]);
    Serial.println(" ");
  }

  //reset array
  for (int l = 1; l < 60; l++) {
    array2[l] = 0;
  }

  Serial.println(" ");
  delay(20000);
}
