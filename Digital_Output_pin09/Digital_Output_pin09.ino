//Global Variable
const int LED = 9;

void setup() {
  pinMode(LED, OUTPUT);
  //digitalWrite(LED, HIGH);
  //Serial : Communication to Monitoring (As Console)
  Serial.begin(9600);
}

void loop() {
  for(int i = 100; i<1000; i+=100){
    digitalWrite(LED, HIGH);
    Serial.println("on");
    delay(i);
    digitalWrite(LED, LOW);
    Serial.println("off");
    delay(i);
    Serial.println(i);
  }
}
