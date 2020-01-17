int digit_pin[] = {5, 4, 3, 2};
int dp = 13;
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
int numLength = 0;
void setup() {
  for (int i = 2 ; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  numLength = String(num,DEC).length() ;
  displayNum(numLength);  
  displayDigit(0);
  delay(1000);
  num ++;
}

void displayNum(int length){
  for(int i = 0 ; i < length ; i++){
    digitalWrite(digit_pin[i], HIGH);
  }
}

void displayDigit(int num) {
  int pin = 6;
  for (int i = 0 ; i < 7 ; i++) {
    digitalWrite(pin + i, digits[num][i]);
  }
}
