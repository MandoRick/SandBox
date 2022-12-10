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

#define testPin0  0
#define testPin1  1
#define testPin2  2
#define testPin3  3
#define testPin4  4
#define testPin5  5
#define testPin6  6
#define testPin7  7
#define testPin8  8
#define testPin9  9
#define testPin10  10
#define testPin11  11
#define testPin12  12
#define testPin13  13
#define testPin14  14
#define testPin15  15
#define testPin16  16
#define testPin17  17
#define testPin18  18
#define testPin19  19
#define testPin20  20
#define testPin21  21
#define testPin22  22
//#define testPin23  23
//#define testPin24  24
#define testPin25  25
#define testPin26  26
#define testPin27  27
#define testPin28  28
int pinDelay = 0;
int pinDelayAmount = 1;


//------- version stuff --------
const String p_project = "timer";
const uint8_t version_hi = 0;
const uint8_t version_lo = 1;
void versionPrint(void) {
  Serial.print("RicksWorx: ");
  Serial.println(p_project);
  Serial.print("Version: ");
  Serial.print(version_hi);
  Serial.print('.');
  Serial.println(version_lo);
}
//------- end version stuff --------

void setup() {
  delay(5000);
  Serial.begin(9600);
  versionPrint();
  pinMode(testPin0, OUTPUT);
  pinMode(testPin1, OUTPUT);
  pinMode(testPin2, OUTPUT);
  pinMode(testPin3, OUTPUT);
  pinMode(testPin4, OUTPUT);
  pinMode(testPin5, OUTPUT);
  pinMode(testPin6, OUTPUT);
  pinMode(testPin7, OUTPUT);
  pinMode(testPin8, OUTPUT);
  pinMode(testPin9, OUTPUT);
  pinMode(testPin10, OUTPUT);
  pinMode(testPin11, OUTPUT);
  pinMode(testPin12, OUTPUT);
  pinMode(testPin13, OUTPUT);
  pinMode(testPin14, OUTPUT);
  pinMode(testPin15, OUTPUT);
}

void setup1() {
  delay(5000);
  pinMode(testPin16, OUTPUT);
  pinMode(testPin17, OUTPUT);
  pinMode(testPin18, OUTPUT);
  pinMode(testPin19, OUTPUT);
  pinMode(testPin20, OUTPUT);
  pinMode(testPin21, OUTPUT);
  pinMode(testPin22, OUTPUT);
  //pinMode(testPin23, OUTPUT);
  //pinMode(testPin24, OUTPUT);
  pinMode(testPin25, OUTPUT);
  pinMode(testPin26, OUTPUT);
  pinMode(testPin27, OUTPUT);
  pinMode(testPin28, OUTPUT);
}

void loop() {
  digitalWrite(testPin0, HIGH);
  delay(pinDelay);
  digitalWrite(testPin0, LOW);
  delay(pinDelay);
  digitalWrite(testPin1, HIGH);
  delay(pinDelay);
  digitalWrite(testPin1, LOW);
  delay(pinDelay);
  digitalWrite(testPin2, HIGH);
  delay(pinDelay);
  digitalWrite(testPin2, LOW);
  delay(pinDelay);
  digitalWrite(testPin3, HIGH);
  delay(pinDelay);
  digitalWrite(testPin3, LOW);
  delay(pinDelay);
  digitalWrite(testPin4, HIGH);
  delay(pinDelay);
  digitalWrite(testPin4, LOW);
  delay(pinDelay);
  digitalWrite(testPin5, HIGH);
  delay(pinDelay);
  digitalWrite(testPin5, LOW);
  delay(pinDelay);
  digitalWrite(testPin6, HIGH);
  delay(pinDelay);
  digitalWrite(testPin6, LOW);
  delay(pinDelay);
  digitalWrite(testPin7, HIGH);
  delay(pinDelay);
  digitalWrite(testPin7, LOW);
  delay(pinDelay);
  digitalWrite(testPin8, HIGH);
  delay(pinDelay);
  digitalWrite(testPin8, LOW);
  delay(pinDelay);
  digitalWrite(testPin9, HIGH);
  delay(pinDelay);
  digitalWrite(testPin9, LOW);
  delay(pinDelay);
  digitalWrite(testPin10, HIGH);
  delay(pinDelay);
  digitalWrite(testPin10, LOW);
  delay(pinDelay);
  digitalWrite(testPin11, HIGH);
  delay(pinDelay);
  digitalWrite(testPin11, LOW);
  delay(pinDelay);
  digitalWrite(testPin12, HIGH);
  delay(pinDelay);
  digitalWrite(testPin12, LOW);
  delay(pinDelay);
  digitalWrite(testPin13, HIGH);
  delay(pinDelay);
  digitalWrite(testPin13, LOW);
  delay(pinDelay);
  digitalWrite(testPin14, HIGH);
  delay(pinDelay);
  digitalWrite(testPin14, LOW);
  delay(pinDelay);
  digitalWrite(testPin15, HIGH);
  delay(pinDelay);
  digitalWrite(testPin15, LOW);
  delay(pinDelay);

  pinDelay = pinDelay + pinDelayAmount;
  if (pinDelay <= 0 || pinDelay >= 16) {
    pinDelayAmount = -pinDelayAmount;
  }

}

void loop1() {
  digitalWrite(testPin16, HIGH);
  delay(pinDelay);
  digitalWrite(testPin16, LOW);
  delay(pinDelay);
  digitalWrite(testPin17, HIGH);
  delay(pinDelay);
  digitalWrite(testPin17, LOW);
  delay(pinDelay);
  digitalWrite(testPin18, HIGH);
  delay(pinDelay);
  digitalWrite(testPin18, LOW);
  delay(pinDelay);
  digitalWrite(testPin19, HIGH);
  delay(pinDelay);
  digitalWrite(testPin19, LOW);
  delay(pinDelay);
  digitalWrite(testPin20, HIGH);
  delay(pinDelay);
  digitalWrite(testPin20, LOW);
  delay(pinDelay);
  digitalWrite(testPin21, HIGH);
  delay(pinDelay);
  digitalWrite(testPin21, LOW);
  delay(pinDelay);
  digitalWrite(testPin22, HIGH);
  delay(pinDelay);
  digitalWrite(testPin22, LOW);
  delay(pinDelay);
  //  digitalWrite(testPin23, HIGH);
  //  delay(pinDelay);
  //  digitalWrite(testPin23, LOW);
  //  delay(pinDelay);
  //  digitalWrite(testPin24, HIGH);
  //  delay(pinDelay);
  //  digitalWrite(testPin24, LOW);
  //  delay(pinDelay);
  digitalWrite(testPin26, HIGH);
  delay(pinDelay);
  digitalWrite(testPin26, LOW);
  delay(pinDelay);
  digitalWrite(testPin27, HIGH);
  delay(pinDelay);
  digitalWrite(testPin27, LOW);
  delay(pinDelay);
  digitalWrite(testPin28, HIGH);
  delay(pinDelay);
  digitalWrite(testPin28, LOW);
  delay(pinDelay);
  digitalWrite(testPin25, HIGH);
  delay(pinDelay);
  digitalWrite(testPin25, LOW);
  delay(pinDelay);
}
