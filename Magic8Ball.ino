/* -------------------
   created by JediRick & Nis
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

const String p_project = "Magic 8 Ball";
const uint8_t version_hi = 0;
const uint8_t version_lo = 3;
const byte charCount = 64;
char serialData[charCount];
const String responses[] = {
  "It is certain",
  "Without a doubt",
  "You may rely on it",
  "Yes definitely",
  "It is decidedly so",
  "As I see it, yes",
  "Most likely",
  "Yes",
  "Outlook good",
  "Signs point to yes",
  "Reply hazy try again",
  "Better not tell you now",
  "Ask again later",
  "Cannot predict now",
  "Concentrate and ask again",
  "Don’t count on it",
  "Outlook not so good",
  "My sources say no",
  "Very doubtful",
  "My reply is no"
};

void versionPrint (void) {
  Serial.print ("RicksWorx: ");
  Serial.println (p_project);
  Serial.print ("Version: ");
  Serial.print (version_hi);
  Serial.print ('.');
  Serial.print (version_lo);
  Serial.print ("  ");
}

void setup () {
  delay (5000);
  Serial.begin (9600);
  versionPrint ();
  randomSeed (analogRead (26));
  Serial.println ("Welcome to Magic 8 ball");
  Serial.println ("Please ask your question");
}

void generateAnswer () {
  int generateRando = random (1, 20);
  //    Serial.print ("  rando: "); // same line as the 'question' put to M8B
  Serial.println ("");
  Serial.println (responses[generateRando]);
}

#define EOL '\n'

void waitForQuestion () {
  char endMarker = EOL;
  char rc;

  while (!Serial.available ()); // hold for input
  do {
    rc = Serial.read ();
    // echo handling
    if (1) { // echo 1  no echo 0
      if (((rc > 31) && (rc < 127)) || (rc == 8)) {
        if (rc == 8) {
          Serial.print (rc);
          Serial.print (' ');
        }
        Serial.print (rc);
      }
    }
  } while (rc != endMarker);
  generateAnswer ();
}

void loop () {
  waitForQuestion ();
}
