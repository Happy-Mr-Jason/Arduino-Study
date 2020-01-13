#define DO_C4 262
#define RE_D4 294
#define MI_E4 330
int pins[] = {2, 3, 4};
int notes[] = {MI_E4, RE_D4, DO_C4};
void setup() {
  for(int i = 0 ; i < 3 ; i++){
    pinMode(pins[i], INPUT);
  }

}

void loop() {
  for(int i = 0 ; i < 3 ; i++){
    if(digitalRead(pins[i]) == HIGH){
      tone(8, notes[i], 20);
    }
  }

}
