//Global Variable
int LED[] = {2, 3, 4, 5, 6};

void setup() {
  pinMode(LED[0], OUTPUT);
  pinMode(LED[1], OUTPUT);
  pinMode(LED[2], OUTPUT);
  pinMode(LED[3], OUTPUT);
  pinMode(LED[4], OUTPUT);
  
  //digitalWrite(LED, HIGH);
  //Serial : Communication to Monitoring (As Console)
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i < 5; i++){
    digitalWrite(LED[i], HIGH);
    Serial.println("on");
    delay(1000);
    digitalWrite(LED[i], LOW);
    Serial.println("off");
    delay(1000);
    Serial.println(i);
  }
}
