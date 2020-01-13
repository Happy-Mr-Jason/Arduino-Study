#define SPK 8
#define PT A0

void setup() {
 Serial.begin(9600);
}

void loop() {
  tone(SPK, map(analogRead(PT), 0, 1023, 31, 4978), 20); 
  Serial.println(analogRead(PT));
  delay(500);
}
