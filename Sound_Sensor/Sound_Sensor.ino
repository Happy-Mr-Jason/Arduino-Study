int volume;
int led=9;
int threshold = 543;
int pin = 7;
void setup() {
  Serial.begin(9600);
  pinMode(pin, INPUT);
}

void loop() {
  volume = analogRead(A0);
  Serial.println(volume,DEC);
  delay(100);
  if(volume > threshold){
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
}
