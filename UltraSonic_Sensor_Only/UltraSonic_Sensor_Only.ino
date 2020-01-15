#define TRIG 3
#define ECHO 2
long distance = 0;

void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(2);
  digitalWrite(TRIG, LOW);
  // ECHO 핀에 초음파가 들어오는 시간을 계산
  // 반환된 값에 58.2를 나눈 이유는 시간을 cm로 변경하기 위함
  distance = pulseIn(ECHO, HIGH) / 58.2;
  Serial.println(distance);
}

void meas_distance(){
  
}
