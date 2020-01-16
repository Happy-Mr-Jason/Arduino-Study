#define BTN_P 11
#define BTN_N 12

//byte digits[10][7] = {
//  {0, 0, 0, 0, 0, 0, 1},
//  {1, 0, 0, 1, 1, 1, 1},
//  {0, 0, 1, 0, 0, 1, 0},
//  {0, 0, 0, 0, 1, 1, 0},
//  {1, 0, 0, 1, 1, 0, 0},
//  {0, 1, 0, 0, 1, 0, 0},
//  {0, 1, 0, 0, 0, 0, 0},
//  {0, 0, 0, 1, 1, 1, 1},
//  {0, 0, 0, 0, 0, 0, 0},
//  {0, 0, 0, 0, 1, 0, 0}};

byte digits[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 1, 0, 0, 1},
  {0, 1, 1, 0, 0, 1, 1},
  {1, 0, 1, 1, 0, 1, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 1, 0, 1, 1}
};

int num = 0;
boolean last_BTN_P = LOW; // Previous Button Status
boolean cur_BTN_P = LOW; // Current Button Staus
boolean last_BTN_N = LOW; // Previous Button Status
boolean cur_BTN_N = LOW; // Current Button Staus


void setup() {
  for (int i = 2 ; i < 10; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(9, HIGH);
  }
  pinMode(BTN_P, INPUT);
  pinMode(BTN_N, INPUT);
  Serial.begin(9600);
}

void loop() {
  cur_BTN_P = debounce(last_BTN_P, BTN_P);
  cur_BTN_N = debounce(last_BTN_N, BTN_N);
  if ( num < 9 ) {
    if (last_BTN_P == LOW && cur_BTN_P == HIGH) {
      num ++;
    }
  }

  if (num > 0 ) {
    if (last_BTN_N == LOW && cur_BTN_N == HIGH) {
      num --;
    }
  }
  last_BTN_P = cur_BTN_P;
  last_BTN_N = cur_BTN_N;
  displayDigit(num);
}

void displayDigit(int num) {
  int pin = 2;
  for (int i = 0 ; i < 7 ; i++) {
    digitalWrite(pin + i, digits[num][i]);
  }
}

boolean debounce(boolean last, int btn){
  boolean current = digitalRead(btn); // Current Button Status
  if(last != current){
    delay(5);
    current = digitalRead(btn);
  }
  return current;
}
