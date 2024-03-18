#define PIN_BUZZER 15

// Notes from A2 to A8
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C2 131
#define NOTE_CS2 139
#define NOTE_D2 147
#define NOTE_DS2 156
#define NOTE_E2 165
#define NOTE_F2 175
#define NOTE_FS2 185
#define NOTE_G2 196
#define NOTE_GS2 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C3 262
#define NOTE_CS3 277
#define NOTE_D3 294
#define NOTE_DS3 311
#define NOTE_E3 330
#define NOTE_F3 349
#define NOTE_FS3 370
#define NOTE_G3 392
#define NOTE_GS3 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C4 523
#define NOTE_CS4 554
#define NOTE_D4 587
#define NOTE_DS4 622
#define NOTE_E4 659
#define NOTE_F4 698
#define NOTE_FS4 740
#define NOTE_G4 784
#define NOTE_GS4 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C5 1047
#define NOTE_CS5 1109
#define NOTE_D5 1175
#define NOTE_DS5 1245
#define NOTE_E5 1319
#define NOTE_F5 1397
#define NOTE_FS5 1480
#define NOTE_G5 1568
#define NOTE_GS5 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C6 2093
#define NOTE_CS6 2217
#define NOTE_D6 2349
#define NOTE_DS6 2489
#define NOTE_E6 2637
#define NOTE_F6 2794
#define NOTE_FS6 2960
#define NOTE_G6 3136
#define NOTE_GS6 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C7 4186
#define NOTE_CS7 4435
#define NOTE_D7 4699
#define NOTE_DS7 4978
#define NOTE_E7 5274
#define NOTE_F7 5588
#define NOTE_FS7 5920
#define NOTE_G7 6272
#define NOTE_GS7 6645
#define NOTE_A8 7040
#define NOTE_AS8 7459
#define NOTE_B8 7902
#define NOTE_C8 8372

#define WHOLE 2200
#define HALF WHOLE / 2
#define QUARTER HALF / 2
#define EIGHTH QUARTER / 2
#define EIGHTH_TRIPLE QUARTER / 3
#define SIXTEENTH EIGHTH / 2
#define THIRTYSECOND SIXTEENTH / 2
#define SIXTYFOURTH THIRTYSECOND / 2
#define HUNDREDTWENTYEIGHTH SIXTYFOURTH / 2
#define TWOHUNREDFIFTYSIXTH HUNDREDTWENTYEIGHTH / 2

void playNote(int frequency, int duration, bool hold = false, bool measure = true) {
  (void)measure;
  if (hold) {
    tone(PIN_BUZZER, frequency, duration + duration / 32);
  } else {
    tone(PIN_BUZZER, frequency, duration);
  }
  delay(duration + duration / 16);
}

void playAsteroidsTune() {
  int melody[] = {
    NOTE_E4, NOTE_D4, NOTE_CS4, NOTE_E4, NOTE_D4, NOTE_CS4, NOTE_E4, NOTE_D4,
    NOTE_CS4, NOTE_E4, NOTE_D4, NOTE_CS4, NOTE_E4, NOTE_D4, NOTE_CS4, NOTE_B3,
    NOTE_E4, NOTE_D4, NOTE_CS4, NOTE_E4, NOTE_D4, NOTE_CS4, NOTE_E4, NOTE_D4,
    NOTE_CS4, NOTE_E4, NOTE_D4, NOTE_CS4, NOTE_E4, NOTE_D4, NOTE_CS4, NOTE_B3
  };

  int noteDurations[] = {
    32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 16,
    32, 32, 32, 32, 32, 32, 32, 32,
    32, 32, 32, 32, 32, 32, 32, 16
  };
  for (int thisNote = 0; thisNote < 32; thisNote++) {
    int noteDuration = WHOLE / noteDurations[thisNote];
    playNote(melody[thisNote], noteDuration, true);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
}

void playShootSound() {
  playNote(NOTE_C8, TWOHUNREDFIFTYSIXTH);
  playNote(NOTE_C7, TWOHUNREDFIFTYSIXTH);
  playNote(NOTE_C6, TWOHUNREDFIFTYSIXTH);
  playNote(NOTE_C5, TWOHUNREDFIFTYSIXTH);
  playNote(NOTE_C4, TWOHUNREDFIFTYSIXTH);
  playNote(NOTE_C3, TWOHUNREDFIFTYSIXTH);
  playNote(NOTE_C2, TWOHUNREDFIFTYSIXTH);
}

void playCrashSound() {
  for (int i = 0; i < 10; i++) {
    playNote(NOTE_A2, TWOHUNREDFIFTYSIXTH);
    playNote(NOTE_A3, TWOHUNREDFIFTYSIXTH);
  }
}
