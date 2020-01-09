#define LED 9
#define BTN 2

void setup() {
  pinMode(LED,OUTPUT);
  pinMode(BTN,INPUT);
  Serial.begin(9800);
}

void loop() {
  
// if(digitalRead(BTN) == HIGH){
//  Serial.println(String("Button On"));
//  digitalWrite(LED, HIGH);
// } else {
//  Serial.println(String("Button Off"));
//  digitalWrite(LED, LOW);
// }

 if(digitalRead(BTN) == HIGH){
  Serial.println(String("Button On"));
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);
 } 
}
