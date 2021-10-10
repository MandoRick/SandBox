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

#define led LED_BUILTIN
byte brightness = 0;
byte fadeAmount = 1;
const byte arrayDigits = 100;
unsigned long array1[arrayDigits];
unsigned long array2[arrayDigits];
unsigned long iteration = 0;
byte frontRunner = 0;

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
  unsigned long biggestNumber = array2[0];
  unsigned long smallestNumber = array2[0];
  unsigned long difference;
  unsigned long averageValueCount;
  unsigned long averageValue;
  Serial.print("[Iteration: " + (String)iteration + "]  [FrontRunner: " + (String)frontRunner + "]  [Core temp: " + (String)analogReadTemp() + "]");
  for (word i = 0; i < arrayDigits; i++) {
    array2[array1[i]]++;
    averageValue = array2[i];
    averageValueCount = averageValueCount + averageValue;
  }
  averageValue = averageValue / arrayDigits;
  Serial.print("  [Average value: " + (String)averageValue + "]");
  for (word i = 0; i < arrayDigits; i++) {
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
  for (word m = 0; m <= 512; m++) {
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
