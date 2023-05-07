

// include the SoftwareSerial library so we can use it to talk to the Emic 2 module
#include <SoftwareSerial.h>

#define rxPin 8             // Serial input (connects to Emic 2's SOUT pin)
#define txPin 9             // Serial output (connects to Emic 2's SIN pin)

// set up a new serial port
SoftwareSerial emicSerial = SoftwareSerial(rxPin, txPin);

String SUBJECT_PRONOUN[7] = { "I", "you", "he", "she", "it", "we", "they" };
String OBJECT_PRONOUN[7] = { "me", "you", "her", "him", "it", "us", "them" };
String DEMONSTRATIVE_PRONOUN[5] = { "this", "that", "these", "those", "such" };
String POSSESSIVE_PRONOUN[6] = { "mine", "yours", "his", "hers", "ours", "theirs" };
String INTERROGATIVE_PRONOUN[9] = { "who", "whoever", "whom", "whomever", "what", "whatever", "which", "whichever", "whose" };
String REFLEXIVE_PRONOUN[8] = { "myself", "yourself", "himself", "herself", "itself", "ourselves", "yourselves", "themselves" };
String RECIPROCAL_PRONOUN[2] = { "each other", "one another" };
String INDEFINITE_PRONOUN[29] = { "all", "another", "any", "anybody", "anyone", "anything", "both", "each", "either", "everybody", "everyone",
                                  "everything", "few", "many", "most", "neither", "nobody", "none", "no-one", "nothing", "one", "other", "others",
                                  "several", "some", "somebody", "someone", "something", "such" };
String RELATIVE_PRONOUN[10] = { "that", "what", "whatever", "which", "whichever", "who", "whoever", "whom", "whomever", "whose" };
String ARCHAIC_PRONOUN[5] = { "thou", "thee", "ye", "thy", "thine" };
String PREPOSITION[70] = { "aboard", "about", "above", "across", "after", "against", "along", "amid", "among", "anti", "around", "as", "at",
                           "before", "behind", "below", "beneath", "beside", "besides", "between", "beyond", "but", "by",
                           "concerning", "considering", "despite", "down", "during",
                           "except", "excepting", "excluding",
                           "following", "for", "from",
                           "in", "inside", "into",
                           "like",
                           "minus",
                           "near",
                           "of", "off", "on", "onto", "opposite", "outside", "over",
                           "past", "per", "plus",
                           "regarding", "round",
                           "save", "since",
                           "than", "through", "to", "toward", "towards",
                           "under", "underneath", "unlike", "until", "up", "upon",
                           "versus", "via",
                           "with", "within", "without" };
String CONJUNCTION[6] = { "and", "but", "or", "nor", "yet", "so" };
String INTERJECTION[31] = { "ah", "alas", "awesome", "bam", "bingo", "blah", "boo", "bravo", "bummer", "cheers", "congratulations",
                            "darn", "doh", "drat", "duh", "eek", "eh", "encore", "eww", "gee", "gosh", "hey", "hooray", "huh",
                            "hurray", "oh", "oops", "ouch", "oy", "phew", "shucks" };
String NUMBER[104] = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten",
                       "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen",
                       "eighteen", "nineteen", "twenty", "twenty-one", "twenty-two", "twenty-three",
                       "twenty-four", "twenty-five", "twenty-six", "twenty-seven", "twenty-eight",
                       "twenty-nine", "thirty", "thirty-one", "thirty-two", "thirty-three", "thirty-four",
                       "thirty-five", "thirty-six", "thirty-seven", "thirty-eight", "thirty-nine", "forty",
                       "forty-one", "forty-two", "forty-three", "forty-four", "forty-five", "forty-six",
                       "forty-seven", "forty-eight", "forty-nine", "fifty", "fifty-one", "fifty-two",
                       "fifty-three", "fifty-four", "fifty-five", "fifty-six", "fifty-seven", "fifty-eight",
                       "fifty-nine", "sixty", "sixty-one", "sixty-two", "sixty-three", "sixty-four", "sixty-five",
                       "sixty-six", "sixty-seven", "sixty-eight", "sixty-nine", "seventy", "seventy-one",
                       "seventy-two", "seventy-three", "seventy-four", "seventy-five", "seventy-six", "seventy-seven",
                       "seventy-eight", "seventy-nine", "eighty", "eighty-one", "eighty-two", "eighty-three",
                       "eighty-four", "eighty-five", "eighty-six", "eighty-seven", "eighty-eight", "eighty-nine",
                       "ninety", "ninety-one", "ninety-two", "ninety-three", "ninety-four", "ninety-five",
                       "ninety-six", "ninety-seven", "ninety-eight", "ninety-nine",
                       "hundred", "thousand", "million", "billion", "trillion" };
String MONTH[12] = { "January", "February", "March", "April", "May", "June",
                     "July", "August", "September", "October", "November", "December" };
String DAY[7] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
String AVERB[20] = { "abandon", "abate", "abbreviate", "abide", "absorb", "abstain", "accelerate", "accept", "accommodate", "accompany", "accuse", "achieve", "acknowledge", "acquire", "act", "adapt", "add", "address", "adjust", "administer" };
String BVERB[20] = { "babble", "back", "backfire", "backpack", "backpedal", "backslide", "backtrack", "badger", "baffle", "bake", "balance", "balk", "ban", "bang", "banish", "banter", "bar", "barbecue", "bargain", "bark" };
String CVERB[20] = { "cache", "calculate", "call", "calm", "camp", "can", "cancel", "candle", "cap", "capitalize", "captivate", "capture", "care", "carve", "cast", "catch", "categorize", "cause", "caution", "celebrate" };
String DVERB[20] = { "dabble", "damage", "dance", "dare", "dash", "dazzle", "deal", "debate", "deceive", "decide", "declare", "decorate", "decrease", "dedicate", "defer", "deflect", "deliver", "demand", "demonstrate", "denote" };
String EVERB[20] = { "eager", "earn", "ease", "eat", "echo", "educate", "elaborate", "elect", "elevate", "eliminate", "embark", "embrace", "emerge", "emphasize", "employ", "empower", "empty", "encircle", "encourage", "endure" };
String FVERB[20] = { "face", "facilitate", "fade", "fail", "faint", "fake", "fall", "falter", "familiarize", "fan", "fancy", "fantasize", "fasten", "favor", "fax", "fear", "feast", "feed", "feel", "fence" };
String GVERB[20] = { "gag", "gain", "gainsay", "gape", "garden", "gargle", "gasp", "gather", "gauge", "gaze", "generalize", "generate", "gesture", "get", "giggle", "give", "glance", "glare", "glean", "glide" };
String HVERB[20] = { "hack", "hail", "halt", "hammer", "hand", "handle", "hang", "happen", "harass", "harden", "harm", "harvest", "hate", "haul", "have", "heal", "heap", "hear", "heat", "help" };
String IVERB[20] = { "identify", "idle", "ignite", "ignore", "illuminate", "illustrate", "imagine", "immerse", "impact", "impair", "impart", "impeach", "implement", "imply", "import", "impose", "impress", "imprint", "improve", "include" };
String JVERB[20] = { "jab", "jam", "jangle", "jaunt", "jazz", "jeer", "jell", "jerk", "jest", "jiggle", "jilt", "jingle", "jinx", "jog", "join", "joke", "jolt", "jostle", "judge", "juggle" };
String KVERB[12] = { "keep", "kick", "kidnap", "kill", "kindle", "kiss", "knead", "kneel", "knit", "knock", "knot", "know" };
String LVERB[20] = { "label", "labor", "lament", "land", "lash", "laugh", "launch", "lavish", "lay", "lead", "lean", "leap", "learn", "leave", "lecture", "lend", "let", "level", "license", "lick" };
String MVERB[20] = { "magnify", "mail", "maintain", "make", "manage", "manipulate", "manufacture", "march", "mark", "market", "marry", "masquerade", "massage", "match", "mate", "matter", "mature", "mean", "measure", "meddle" };
String NVERB[20] = { "nail", "name", "nap", "navigate", "need", "neglect", "negotiate", "nest", "nod", "nominate", "normalize", "note", "notice", "nourish", "nudge", "nullify", "number", "nurse", "nurture" };
String OVERB[20] = { "obey", "object", "oblige", "observe", "obstruct", "obtain", "occupy", "occur", "offend", "offer", "offset", "omit", "open", "operate", "oppose", "opt", "orchestrate", "order", "organize", "originate" };
String PVERB[20] = { "pack", "paint", "pamper", "panic", "parade", "park", "participate", "pass", "paste", "pat", "patch", "patrol", "pause", "pay", "peck", "pedal", "peel", "peer", "penetrate", "perceive" };
String QVERB[11] = { "qualify", "quantify", "quarrel", "query", "question", "quicken", "quiet", "quintuple", "quip", "quit", "quiz" };
String RVERB[20] = { "race", "radiate", "raise", "rattle", "reach", "read", "realize", "reap", "rearrange", "reason", "rebel", "recall", "recapture", "receive", "recite", "recognize", "recommend", "reconcile", "reconstruct", "record" };
String SVERB[20] = { "sabotage", "sack", "sacrifice", "sadden", "sail", "salivate", "salute", "sample", "sanction", "sashay", "sass", "sate", "satiate", "satisfy", "saturate", "saunter", "save", "savor", "scald", "scale" };
String TVERB[20] = { "tabulate", "tackle", "tag", "tailor", "take", "tame", "tangle", "tantalize", "tap", "taper", "target", "taste", "taunt", "teach", "tear", "tease", "teleport", "tell", "tempt", "tend" };
String UVERB[20] = { "ululate", "umpire", "unbend", "uncover", "undo", "unify", "unleash", "unload", "unlock", "unravel", "untangle", "unveil", "uphold", "uplift", "upset", "urge", "use", "usher", "usurp", "utilize" };
String VVERB[20] = { "vacate", "validate", "vanish", "vault", "veer", "vent", "venture", "verify", "vex", "visit", "visualize", "vitalize", "vocalize", "vomit", "vote", "vouch", "vow", "voyage", "vulcanize", "vulgarize" };
String WVERB[20] = { "wade", "wag", "wail", "wait", "wake", "walk", "wallow", "wander", "want", "warm", "warn", "warrant", "wash", "waste", "watch", "water", "wave", "waver", "wear", "weave" };
String XVERB[20] = { "x-ray", "xerox", "x-ray", "xenotransplant", "xeriscape", "x-ray", "xhosa", "x-ray", "x-out", "x-ray", "xenophonize", "xenogenize", "xenograft", "xerography", "x-ray", "x-ray", "x-ray", "x-ray", "x-ray", "x-ray" };
String YVERB[20] = { "yammer", "yank", "yap", "yawn", "yearn", "yell", "yield", "yip", "yodel", "yoke", "youngerize", "yowl", "yup", "yack", "yacht", "yak", "yen", "yellow", "yell", "yelp" };
String ZVERB[20] = { "zero", "zigzag", "zing", "zip", "zone", "zoom", "zest", "zeroed", "zigged", "zipped", "zapped", "zoned", "zoomed", "zested", "zeal", "zinc", "zone in", "zapped out", "zoom in", "zero out" };




void setup() {
  randomSeed(analogRead(A0));
  setupEmic();
}

void loop() {
  // Speak some text
  emicSerial.print('S');
  emicSerial.print(SUBJECT_PRONOUN[random(7)] + ".");
  emicSerial.print(OBJECT_PRONOUN[random(7)] + ".");
  emicSerial.print(DEMONSTRATIVE_PRONOUN[random(5)] + ".");
  emicSerial.print(POSSESSIVE_PRONOUN[random(6)] + ".");
  emicSerial.print(INTERROGATIVE_PRONOUN[random(9)] + ".");
  emicSerial.print(REFLEXIVE_PRONOUN[random(8)] + ".");
  emicSerial.print(RECIPROCAL_PRONOUN[random(2)] + ".");
  emicSerial.print(INDEFINITE_PRONOUN[random(29)] + ".");
  emicSerial.print(RELATIVE_PRONOUN[random(10)] + ".");
  emicSerial.print(ARCHAIC_PRONOUN[random(5)] + ".");
  emicSerial.print(PREPOSITION[random(70)] + ".");
  emicSerial.print(CONJUNCTION[random(6)] + ".");
  emicSerial.print(INTERJECTION[random(31)] + ".");
  emicSerial.print(NUMBER[random(104)] + ".");
  emicSerial.print(MONTH[random(12)] + ".");
  emicSerial.print(DAY[random(7)] + ".");
  emicSerial.print(AVERB[random(20)] + ".");
  emicSerial.print(BVERB[random(20)] + ".");
  emicSerial.print(CVERB[random(20)] + ".");
  emicSerial.print(DVERB[random(20)] + ".");
  emicSerial.print(EVERB[random(20)] + ".");
  emicSerial.print(FVERB[random(20)] + ".");
  emicSerial.print(GVERB[random(20)] + ".");
  emicSerial.print(HVERB[random(20)] + ".");
  emicSerial.print(IVERB[random(20)] + ".");
  emicSerial.print(JVERB[random(20)] + ".");
  emicSerial.print(KVERB[random(12)] + ".");
  emicSerial.print(LVERB[random(20)] + ".");
  emicSerial.print(MVERB[random(20)] + ".");
  emicSerial.print(NVERB[random(20)] + ".");
  emicSerial.print(OVERB[random(20)] + ".");
  emicSerial.print(PVERB[random(20)] + ".");
  emicSerial.print(QVERB[random(11)] + ".");
  emicSerial.print(RVERB[random(20)] + ".");
  emicSerial.print(SVERB[random(20)] + ".");
  emicSerial.print(TVERB[random(20)] + ".");
  emicSerial.print(UVERB[random(20)] + ".");
  emicSerial.print(VVERB[random(20)] + ".");
  emicSerial.print(WVERB[random(20)] + ".");
  emicSerial.print(XVERB[random(20)] + ".");
  emicSerial.print(YVERB[random(20)] + ".");
  emicSerial.print(ZVERB[random(20)] + ".");
  emicSerial.flush();
  delay(5000);
  /*
  while (!emicSerial.read()) {
    // do nothing while waiting
  }


  while (emicSerial.read() != ':')
    ;            // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command
  delay(25000);  // 1/2 second delay
  //emicSerial.flush();
*/
}

void setupEmic() {
  // define pin modes
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  emicSerial.begin(9600);
  delay(5000);
  emicSerial.print('\n');  // Send a CR in case the system is already up
  while (emicSerial.read() != ':')
    ;                  // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
  delay(10);           // Short delay
  emicSerial.flush();  // Flush the receive buffer
}