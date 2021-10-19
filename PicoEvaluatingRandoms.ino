/* -------------------
   created by JediRick
   -------------------
  boolean        (8 bit)   -  true/false
  byte           (8 bit)   -  unsigned number from 0-255
  char           (8 bit)   -  signed number from -128 to 127
  unsigned char  (8 bit)   -  same as byte
  word           (16 bit)  -  unsigned number from 0-65535
  unsigned int   (16 bit)  -  the same as word
  int            (16 bit)  -  signed number from -32768 to 32767
  unsigned long  (32 bit)  -  unsigned number from 0-4,294,967,295  Usually for millis
  long           (32 bit)  -  signed number from -2,147,483,648 to 2,147,483,647
  float          (32 bit)  -  signed number from -3.4028235E38 to 3.4028235E38  Floating point not native so avoid if you can
  uint8_t        (8 bit)   -  [0-255]
  int8_t         (8 bit)   -  [-127 - 127]
  uint16_t       (16 bit)  -  [0-65,535]
  int16_t        (16 bit)  -  [-32,768 - 32,767]
  uint32_t       (32 bit)  -  [0-4,294,967,295]
  int32_t        (32 bit)  -  [-2,147,483,648 - 2,147,483,647]
  uint64_t       (64 bit)  -  [0-18,446,744,073,709,551,615]  
  int64_t        (64 bit)  -  [−9,223,372,036,854,775,808 - 9,223,372,036,854,775,807]
*/

#define led LED_BUILTIN
uint8_t brightness = 0;
uint8_t fadeAmount = 1;
const uint8_t arrayDigits = 100;
uint32_t array1[arrayDigits];
uint32_t array2[arrayDigits];
uint32_t iteration = 0;
uint16_t frontRunner = 0;

void setup() {
  delay(2000);
  randomSeed(analogRead(27));
  Serial.begin(115200);
  pinMode(led, OUTPUT);
}

void setup1() {
  delay(2000);
}

void loop() {
  makeNumber();
  addThemUp();
  iteration++;
  delay(10);
}

void loop1() {
  fadeLed();
}

void makeNumber() {
  for (word i = 0; i < arrayDigits; i++) {
    array1[i] = random(arrayDigits);
  }
}

void addThemUp() {
  uint32_t biggestNumber = array2[0];
  uint32_t smallestNumber = array2[0];
  uint32_t difference;
  uint32_t averageValueCount;
  uint32_t averageValue;
  Serial.print("[Iteration: " + (String)iteration + "]  [FrontRunner: " + (String)frontRunner + "]  [Core temp: " + (String)analogReadTemp() + "]");
  for (uint16_t i = 0; i < arrayDigits; i++) {
    array2[array1[i]]++;
    averageValue = array2[i];
    averageValueCount = averageValueCount + averageValue;
  }
  averageValue = averageValue / arrayDigits;
  Serial.print("  [Average value: " + (String)averageValue + "]");
  for (uint16_t i = 0; i < arrayDigits; i++) {
    if (biggestNumber < array2[i]) {
      biggestNumber = array2[i];
      frontRunner = i;
    }
    if (smallestNumber > array2[i]) {
      smallestNumber = array2[i];
    }
  }
  difference = biggestNumber - smallestNumber;
  Serial.println("  [Difference: " + (String)difference + "]");
  biggestNumber = 0;
  smallestNumber = 0;
}

void fadeLed() {
  for (uint16_t m = 0; m <= 512; m++) {
    analogWrite(led, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    delay(5);
  }
  brightness = 0;
  analogWrite(led, 0);
}
