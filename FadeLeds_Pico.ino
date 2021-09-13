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

#define ledRed 1
#define ledGreen 2
#define ledBlue 3
#define ledBuiltin LED_BUILTIN
const String p_project = "xxx";
const uint8_t version_hi = 0;
const uint8_t version_lo = 1;
int brightness = 0;
int fadeAmount = 1;

void versionPrint(void) {
  Serial.print("RicksWorx: ");
  Serial.println(p_project);
  Serial.print("Version: ");
  Serial.print(version_hi);
  Serial.print('.');
  Serial.println(version_lo);
}


void setup() {
  delay(2000);
  Serial.begin(115200);
  versionPrint();
  randomSeed(analogRead(26));
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledBlue, OUTPUT);
}

void setup1() {
  delay(2000);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  fadeRed();
  fadeGreen();
  fadeBlue();
  discoLights();
}

void loop1() {
  fadeOnboard();
}

void fadeOnboard() {
  for (int x = 0; x <= 255; x++) {
    analogWrite(LED_BUILTIN, x);
    delay(10);
  }
  delay(100);
  for (int x = 255; x >= 0; x--) {
    analogWrite(LED_BUILTIN, x);
    delay(10);
  }
  delay(100);
}

void fadeRed() {
  Serial.println("Fading RED");
  for (int i = 0; i < 512; i++) {
    analogWrite(ledRed, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    delay(5);
  }
  analogWrite(ledRed, 0);
  brightness = 0;
}

void fadeGreen() {
  Serial.println("Fading GREEN");
  for (int i = 0; i < 512; i++) {
    analogWrite(ledGreen, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    delay(5);
  }
  analogWrite(ledGreen, 0);
  brightness = 0;
}

void fadeBlue() {
  Serial.println("Fading BLUE");
  for (int i = 0; i < 512; i++) {
    analogWrite(ledBlue, brightness);
    brightness = brightness + fadeAmount;
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    delay(5);
  }
  analogWrite(ledBlue, 0);
  brightness = 0;
}

void discoLights() {
  int randNumber;
  for (int i = 0; i < 20; i++) {
    randNumber = random(200);
    analogWrite(ledRed, randNumber);
    randNumber = random(200);
    analogWrite(ledGreen, randNumber);
    randNumber = random(200);
    analogWrite(ledBlue, randNumber);
    randNumber = random(250, 750);
    delay(randNumber);
  }
  analogWrite(ledRed, 0);
  analogWrite(ledGreen, 0);
  analogWrite(ledBlue, 0);
}
