// Macro Constant (As Final Variable)
#define DELAY_TIME 100

void setup() {
  for(int i = 2 ; i <= 6; i++){
    pinMode(i,OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  digitalWrite(2, HIGH);
  delay(DELAY_TIME);
  digitalWrite(2, LOW);
  delay(DELAY_TIME);
  digitalWrite(3, HIGH);
  delay(DELAY_TIME);
  digitalWrite(3, LOW);
  delay(DELAY_TIME);
  digitalWrite(4, HIGH);
  delay(DELAY_TIME);
  digitalWrite(4, LOW);
  delay(DELAY_TIME);
  digitalWrite(5, HIGH);
  delay(DELAY_TIME);
  digitalWrite(5, LOW);
  delay(DELAY_TIME);
  digitalWrite(6, HIGH);
  delay(DELAY_TIME);
  digitalWrite(6, LOW);
  delay(DELAY_TIME);
}
