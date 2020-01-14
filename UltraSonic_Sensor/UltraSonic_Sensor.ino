#define TRIG 3
#define ECHO 2
#define RED 11
#define GREEN 10
#define BLUE 9

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  Ultra_Trigger(TRIG);
  // ECHO 핀에 초음파가 들어오는 시간을 계산
  // 반환된 값에 58.2를 나눈 이유는 시간을 cm로 변경하기 위함
  long distance = pulseIn(ECHO, HIGH) / 58.2;
  Serial.println(distance);
  analogWrite(RED, 0);
  analogWrite(GREEN, 0);
  analogWrite(BLUE, 0);
  if(distance < 10){
    analogWrite(RED, 255);
  }else if(distance < 20){
    analogWrite(GREEN, 255);
  }else if(distance < 30){
    analogWrite(BLUE, 255);
  }
}

void Ultra_Trigger(int trig){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(2);
  digitalWrite(trig, LOW);
}
