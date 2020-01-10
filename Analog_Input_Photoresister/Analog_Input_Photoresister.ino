int POT = A1;
int LED = 9;
int val = 0;
int bright = 0;
void setup() {
  pinMode(POT, INPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
    val = analogRead(POT);
    bright = map(val, 0, 1023, 255, 0);
    if(val > 400){
      bright = 0;    
    }

    analogWrite(LED, bright);
    Serial.println(String("광량 : ") + val + String(" 밝기 : ") + bright);
}
