int POT = A0;
int LED = 3;
int volume = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(POT, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  volume = analogRead(POT);
  int led_Brightness = volume / 4;
  analogWrite(LED, led_Brightness);
  Serial.println(String("밝기 : ") + led_Brightness);
  delay(100);
}
