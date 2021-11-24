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

#define debug 1     // 1 ON 0 OFF
#if debug == 1
#define debugln(x) Serial.println(x)
#define debug(x) Serial.print(x)
#else
#define debugln(x)
#define debug(x)
#endif
#define ledPin  LED_BUILTIN

//------- version stuff --------
const String p_project = "default";
const uint8_t version_hi = 0;
const uint8_t version_lo = 1;
void versionPrint(void) {
  debug("RicksWorx: ");
  debugln(p_project);
  debug("Version: ");
  debug(version_hi);
  debug(".");
  debugln(version_lo);
}
//------- end version stuff --------

void setup() {
  delay(2000);
  Serial.begin(9600);
  debugln("Serial started");
  versionPrint();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  debugln("Start of main loop");
}