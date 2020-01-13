#include "pitches.h"

int melody[] = {NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4,
                NOTE_G4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, 0,
                NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, NOTE_G4, NOTE_E4,
                NOTE_G4, NOTE_E4, NOTE_D4, NOTE_E4, NOTE_C4, 0
               };

int noteDurations[] = {1, 1, 1, 1, 1, 1, 2,
                       1, 1, 1, 1, 3, 1,
                       1, 1, 1, 1, 1, 2, 1,
                       1, 1, 1, 1, 1, 3
                      };

void setup() {
  
}

void loop() {
  for (int i = 0 ; i < sizeof(melody)/sizeof(int) ; i++) {
    int noteDuration = 250 * noteDurations[i];
    tone(8, melody[i], noteDuration);
    int pause = noteDuration * 1.30;
    delay(pause);
    noTone(8);
  }
}
