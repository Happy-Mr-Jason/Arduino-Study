int waterPin = A0;
int led = 9;
int val = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  val = analogRead(waterPin);
  if(val > 100) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  Serial.println(val);
  delay(500);

}
