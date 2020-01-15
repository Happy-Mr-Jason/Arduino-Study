int reading;
int lmPin = A2;
float temperature;
void setup() {
  analogReference(INTERNAL);
  Serial.begin(9600);
}

void loop() {
  reading = analogRead(lmPin);
  temperature = reading / 9.31;
  Serial.println(temperature);
}
