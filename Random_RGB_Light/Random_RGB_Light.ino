#define RED 11
#define GREEN 10
#define BLUE 9

void setup() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  randomSeed(analogRead(0)); //난수 초기화
//  Serial.begin(9600);
}

void loop() {
  analogWrite(RED, random(255)); // 0~254 무작위 수 발생
  analogWrite(GREEN, random(255));
  analogWrite(BLUE, random(255));
  delay(50);
}

void meas_distance(){
  
}
