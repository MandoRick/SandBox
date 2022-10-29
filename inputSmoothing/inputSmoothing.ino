/*-data-type------size---------description----------------------
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
  --------------------------------------------------------------
  camelCase                -  anything that changes
  snake_case               -  variable's that are exclusive in a function
  Snake_Case               -  CLASS/struct exclusave varables/functions
  iNVERTEDcAMELcASE        -  outside code that is being accessed [database]
  SNake_CAse               -  duplicate varables inside the case function [frequently used in library names]
  ALL_CAPS                 -  const varable names or defines
  ------------- by jediRick & RefreshMyMind --------------------
*/

#define inputPin1 (A0)
#define inputPin2 (A1)
#define inputPin3 (A2)

#define DATA_DIGITS (3)  //how many data inputs
#define DATA_AXIS (20)   //how many smoothing iterations

int16_t DataIn[DATA_AXIS][DATA_DIGITS];
int16_t DataOut[DATA_DIGITS];
int16_t smoothingIndex = 0;

void setup() {
  Serial.begin(115200);
  setupSensors();
}

void setupSensors() {
}

void loop() {
  collect_data();
  parse_data();
}

void collect_data(void) {
  for (uint16_t t = 0; t < DATA_DIGITS; t++) {
    if (t == 0) {
      DataIn[smoothingIndex][t] = analogRead(inputPin1);
    } else if (t == 1) {
      DataIn[smoothingIndex][t] = analogRead(inputPin2);
    } else if (t == 2) {
      DataIn[smoothingIndex][t] = analogRead(inputPin3);
    } 
  }
  smoothingIndex++;
  if (smoothingIndex > DATA_AXIS) {
    smoothingIndex = 0;
  }
  for (uint16_t u = 0; u < DATA_DIGITS; u++) {
    Serial.print(DataOut[u]);
    Serial.print(", ");
  }
  Serial.println();
}

void parse_data(void) {
  for (uint16_t i = 0; i < DATA_DIGITS; i++) {
    int16_t averageValue = 0;
    for (uint16_t j = 0; j < DATA_AXIS; j++) {
      averageValue = averageValue + DataIn[j][i];
    }
    averageValue = averageValue / DATA_AXIS;
    DataOut[i] = averageValue;
  }
}