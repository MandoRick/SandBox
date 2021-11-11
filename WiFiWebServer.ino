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

#include <SPI.h>
#include <WiFiNINA.h>
#define ledPin  LED_BUILTIN

char ssid[] = "wifi_id";
char pass[] = "password";
bool rainbowActive = false;
word led1Brightness = 0;
word fadeAmount1 = 1;
word led2Brightness = 125;
word fadeAmount2 = 1;
word led3Brightness = 254;
word fadeAmount3 = 1;

int status = WL_IDLE_STATUS;
WiFiServer server(80);

//------- version stuff --------
const String p_project = "Ricks Wifi Server";
const uint8_t version_hi = 0;
const uint8_t version_lo = 1;
void versionPrint(void) {
  Serial.print("JediWorx: ");
  Serial.println(p_project);
  Serial.print("Version: ");
  Serial.print(version_hi);
  Serial.print('.');
  Serial.println(version_lo);
}
//------- end version stuff --------

void setup() {
  delay(5000);
  versionPrint();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  analogWrite(LEDR, 255);
  analogWrite(LEDG, 255);
  analogWrite(LEDB, 255);
  Serial.begin(115200);
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  server.begin();
  printWifiStatus();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("new client");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.print(
              "<style>"
              "body{-webkit-animation: colorchange 20s infinite; animation: colorchange 20s infinite;}@-webkit-keyframes colorchange {0% {background:#121212;}25% {background: #1c1b1b;}50% {background:#121214;}75% {background: #1c1b1b;}100% {background: #121212;}}@keyframes colorchange {0% {background:#121212;}25% {background: #1c1b1b;}50% {background: #121214;}75% {background: #1c1b1b;}100% {background:#121212;}}"
              ".container {margin: 0 auto; text-align: center; margin-top: 100px;}"
              "button {color: white; width: 80px; height: 80px;"
              "border-radius: 25%; margin: 20px; border: none; font-size: 20px; outline: none; transition: all 0.2s;}"
              ".red{background-color: rgb(255, 0, 0);}"
              ".green{background-color: rgb(0, 255, 0);}"
              ".blue{background-color: rgb(0, 0, 255);}"
              ".pink{background-color: rgb(255, 0, 255);}"
              ".yellow{background-color: rgb(255, 255, 0);}"
              ".aqua{background-color: rgb(0, 255, 255);}"
              ".mix{background-color: rgb(60, 60, 60);}"
              ".off{background-color: rgb(28, 28, 28);}"
              "button:hover{cursor: pointer; opacity: 0.7;}"
              "</style>"
              "<p style=\"font-size: 30pt; border:2px solid DodgerBlue;color:DodgerBlue;text-align:center;\">RICKs LEDs</p>"
              "<div class='container'>"
              "<button class='red' type='submit' onmousedown='location.href=\"RH\"'>ON</button>"
              "<button class='green' type='submit' onmousedown='location.href=\"GH\"'>ON</button>"
              "<button class='blue' type='submit' onmousedown='location.href=\"BH\"'>ON</button>"
              "<button class='pink' type='submit' onmousedown='location.href=\"PH\"'>ON</button>"
              "<button class='yellow' type='submit' onmousedown='location.href=\"YH\"'>ON</button>"
              "<button class='aqua' type='submit' onmousedown='location.href=\"AH\"'>ON</button><br>"
              "<button class='mix' type='submit' onmousedown='location.href=\"MH\"'>MIX</button><br>"
              "<button class='off' type='submit' onmousedown='location.href=\"LL\"'>OFF</button><br>"

              "</div>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /RH")) {
          rainbowActive = false;
          analogWrite(LEDR, 255);
          analogWrite(LEDG, 255);
          analogWrite(LEDB, 255);
          analogWrite(LEDR, 0);
        }
        if (currentLine.endsWith("GET /GH")) {
          rainbowActive = false;
          analogWrite(LEDR, 255);
          analogWrite(LEDG, 255);
          analogWrite(LEDB, 255);
          analogWrite(LEDG, 0);
        }
        if (currentLine.endsWith("GET /BH")) {
          rainbowActive = false;
          analogWrite(LEDR, 255);
          analogWrite(LEDG, 255);
          analogWrite(LEDB, 255);
          analogWrite(LEDB, 0);
        }
        if (currentLine.endsWith("GET /PH")) {
          rainbowActive = false;
          analogWrite(LEDR, 255);
          analogWrite(LEDG, 255);
          analogWrite(LEDB, 255);
          analogWrite(LEDR, 0);
          analogWrite(LEDB, 0);
        }
        if (currentLine.endsWith("GET /YH")) {
          analogWrite(LEDR, 255);
          analogWrite(LEDG, 255);
          analogWrite(LEDB, 255);
          analogWrite(LEDR, 0);
          analogWrite(LEDG, 0);
        }
        if (currentLine.endsWith("GET /AH")) {
          rainbowActive = false;
          analogWrite(LEDR, 255);
          analogWrite(LEDG, 255);
          analogWrite(LEDB, 255);
          analogWrite(LEDB, 0);
          analogWrite(LEDG, 0);
        }
        if (currentLine.endsWith("GET /MH")) {
          rainbowActive = true;
        }
        if (currentLine.endsWith("GET /LL")) {
          rainbowActive = false;
          analogWrite(LEDR, 255);
          analogWrite(LEDG, 255);
          analogWrite(LEDB, 255);
        }
      }
    }
    client.stop();
    Serial.println("client disconnected");
  }
  if (rainbowActive) {
    rainbowFade();
  }
  delay(5);
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
}


void rainbowFade() {
  analogWrite(LEDR, led1Brightness);
  analogWrite(LEDG, led2Brightness);
  analogWrite(LEDB, led3Brightness);
  led1Brightness = led1Brightness + fadeAmount1;
  led2Brightness = led2Brightness + fadeAmount2;
  led3Brightness = led3Brightness + fadeAmount3;
  if (led1Brightness <= 0 || led1Brightness >= 255) {
    fadeAmount1 = -fadeAmount1;
  }
  if (led2Brightness <= 0 || led2Brightness >= 255) {
    fadeAmount2 = -fadeAmount2;
  }
  if (led3Brightness <= 0 || led3Brightness >= 255) {
    fadeAmount3 = -fadeAmount3;
  }
  if (led1Brightness == 255) {
    led1Brightness = random(0, 255);
  }
  if (led2Brightness == 255) {
    led2Brightness = random(0, 255);
  }
  if (led3Brightness == 255) {
    led3Brightness = random(0, 255);
  }
}
