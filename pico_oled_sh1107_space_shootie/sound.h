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


// Define note durations.  You only need to adjust the whole note
// time and other notes will be subdivided from it directly.
#define WHOLE 2200  // Length of time in milliseconds of a whole note (i.e. a full bar).
#define HALF WHOLE / 2
#define QUARTER HALF / 2
#define EIGHTH QUARTER / 2
#define EIGHTH_TRIPLE QUARTER / 3
#define SIXTEENTH EIGHTH / 2
#define THIRTYSECOND SIXTEENTH / 2
#define SIXTYFOURTH THIRTYSECOND / 2
#define HUNDREDTWENTYEIGHTH SIXTYFOURTH / 2
#define TWOHUNREDFIFTYSIXTH HUNDREDTWENTYEIGHTH / 2

// Play a note of the specified frequency and for the specified duration.
// Hold is an optional bool that specifies if this note should be held a
// little longer, i.e. for eighth notes that are tied together.
// While waiting for a note to play the waitBreath delay function is used
// so breath detection and pixel animation continues to run.  No tones
// will play if the slide switch is in the -/off position or all the
// candles have been blown out.
void playNote(int frequency, int duration, bool hold = false, bool measure = true) {
  (void)measure;

  if (hold) {
    // For a note that's held play it a little longer than the specified duration
    // so it blends into the next tone (but there's still a small delay to
    // hear the next note).
    tone(PIN_BUZZER, frequency, duration + duration / 32);
  } else {
    // For a note that isn't held just play it for the specified duration.
    tone(PIN_BUZZER, frequency, duration);
  }

  delay(duration + duration / 16);
}

// Song to play when the candles are blown out.
void celebrateSong() {
  // Play a little charge melody, from:
  //  https://en.wikipedia.org/wiki/Charge_(fanfare)
  // Note the explicit boolean parameters in particular the measure=false
  // at the end.  This means the notes will play without any breath measurement
  // logic.  Without this false value playNote will try to keep waiting for candles
  // to blow out during the celebration song!
  playNote(NOTE_G4, EIGHTH_TRIPLE, true, false);
  playNote(NOTE_C5, EIGHTH_TRIPLE, true, false);
  playNote(NOTE_E5, EIGHTH_TRIPLE, false, false);
  playNote(NOTE_G5, EIGHTH, true, false);
  playNote(NOTE_E5, SIXTEENTH, false);
  playNote(NOTE_G5, HALF, false);
}


void shootieSound() {
  // Play happy birthday tune, from:
  //  http://www.irish-folk-songs.com/happy-birthday-tin-whistle-sheet-music.html#.WXFJMtPytBw
  // Inside each playNote call it will play a note and drive the NeoPixel animation
  // and check for a breath against the sound sensor.  Once all the candles are blown out
  // the playNote calls will stop playing music.
  playNote(NOTE_D4, EIGHTH, true);
  playNote(NOTE_D4, EIGHTH);
  playNote(NOTE_E4, QUARTER);  // Bar 1
  playNote(NOTE_D4, QUARTER);
  playNote(NOTE_G4, QUARTER);
  playNote(NOTE_FS4, HALF);  // Bar 2
  playNote(NOTE_D4, EIGHTH, true);
  playNote(NOTE_D4, EIGHTH);
  playNote(NOTE_E4, QUARTER);  // Bar 3
  playNote(NOTE_D4, QUARTER);
  playNote(NOTE_A4, QUARTER);
  playNote(NOTE_G4, HALF);  // Bar 4
  playNote(NOTE_D4, EIGHTH, true);
  playNote(NOTE_D4, EIGHTH);
  playNote(NOTE_D5, QUARTER);  // Bar 5
  playNote(NOTE_B4, QUARTER);
  playNote(NOTE_G4, QUARTER);
  playNote(NOTE_FS4, QUARTER);  // Bar 6
  playNote(NOTE_E4, QUARTER);
  playNote(NOTE_C5, EIGHTH, true);
  playNote(NOTE_C5, EIGHTH);
  playNote(NOTE_B4, QUARTER);  // Bar 7
  playNote(NOTE_G4, QUARTER);
  playNote(NOTE_A4, QUARTER);
  playNote(NOTE_G4, HALF);  // Bar 8
}

void testSound() {
  // Define durations for testing
  int durations[] = { SIXTEENTH, EIGHTH, QUARTER, HALF, WHOLE };

  // Iterate over each duration
  for (int durIndex = 0; durIndex < 5; durIndex++) {
    int duration = durations[durIndex];
    Serial.print("Playing notes with duration: ");
    Serial.print(duration);
    Serial.println("ms");

    // Iterate over each note
    for (int noteIndex = 0; noteIndex < 47; noteIndex++) {
      int note;
      switch (noteIndex) {
        case 0:
          note = NOTE_A3;
          break;
        case 1:
          note = NOTE_AS3;
          break;
        case 2:
          note = NOTE_G3;
          break;
        case 3:
          note = NOTE_GS3;
          break;
        case 4:
          note = NOTE_F3;
          break;
        case 5:
          note = NOTE_FS3;
          break;
        case 6:
          note = NOTE_E3;
          break;
        case 7:
          note = NOTE_DS3;
          break;
        case 8:
          note = NOTE_D3;
          break;
        case 9:
          note = NOTE_C3;
          break;
        case 10:
          note = NOTE_B3;
          break;
        case 11:
          note = NOTE_C4;
          break;
        case 12:
          note = NOTE_CS4;
          break;
        case 13:
          note = NOTE_D4;
          break;
        case 14:
          note = NOTE_DS4;
          break;
        case 15:
          note = NOTE_E4;
          break;
        case 16:
          note = NOTE_F4;
          break;
        case 17:
          note = NOTE_FS4;
          break;
        case 18:
          note = NOTE_G4;
          break;
        case 19:
          note = NOTE_GS4;
          break;
        case 20:
          note = NOTE_A4;
          break;
        case 21:
          note = NOTE_AS4;
          break;
        case 22:
          note = NOTE_B4;
          break;
        case 23:
          note = NOTE_C5;
          break;
        case 24:
          note = NOTE_CS5;
          break;
        case 25:
          note = NOTE_D5;
          break;
        case 26:
          note = NOTE_DS5;
          break;
        case 27:
          note = NOTE_E5;
          break;
        case 28:
          note = NOTE_F5;
          break;
        case 29:
          note = NOTE_FS5;
          break;
        case 30:
          note = NOTE_G5;
          break;
        case 31:
          note = NOTE_GS5;
          break;
        case 32:
          note = NOTE_A5;
          break;
        case 33:
          note = NOTE_AS5;
          break;
        case 34:
          note = NOTE_B5;
          break;
        case 35:
          note = NOTE_C6;
          break;
        case 36:
          note = NOTE_CS6;
          break;
        case 37:
          note = NOTE_D6;
          break;
        case 38:
          note = NOTE_DS6;
          break;
        case 39:
          note = NOTE_E6;
          break;
        case 40:
          note = NOTE_F6;
          break;
        case 41:
          note = NOTE_FS6;
          break;
        case 42:
          note = NOTE_G6;
          break;
        case 43:
          note = NOTE_GS6;
          break;
        case 44:
          note = NOTE_A6;
          break;
        case 45:
          note = NOTE_AS6;
          break;
        case 46:
          note = NOTE_B6;
          break;
        case 47:
          note = NOTE_C7;
          break;
      }

      tone(PIN_BUZZER, note, duration);
      delay(duration + duration / 16);  // Pause between notes
      noTone(PIN_BUZZER);
    }
  }
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

  // Iterate over the notes
  for (int thisNote = 0; thisNote < 32; thisNote++) {
    // Calculate the duration of each note
    int noteDuration = WHOLE / noteDurations[thisNote];
    playNote(melody[thisNote], noteDuration, true);

    // Pause between notes
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
