#define LED_L 8
#define LED_R 10
#define LED_F 9
#define LED_B 11

void setup() {
  pinMode(LED_L, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_F, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int push = digitalRead(3);
  int posX = analogRead(A0);
  int posY = analogRead(A1);
  Serial.print(String("버튼상태 : ") + push);
  Serial.print(String(" X값 : ") + posX); 
  Serial.println(String(" Y값 : ") + posY);
  
  if(posX < 400){
    digitalWrite(LED_B, HIGH);
  }else if (posX > 600) {
    digitalWrite(LED_F, HIGH);
  }else{
    digitalWrite(LED_F, LOW);
    digitalWrite(LED_B, LOW);
  }

  if(posY < 400){
    digitalWrite(LED_L, HIGH);
  }else if (posY > 600) {
    digitalWrite(LED_R, HIGH);
  }else{
    digitalWrite(LED_L, LOW);
    digitalWrite(LED_R, LOW);
  }

  if(push == 0){
    digitalWrite(LED_L, HIGH);
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_F, HIGH);
    digitalWrite(LED_B, HIGH);
  } 
}
