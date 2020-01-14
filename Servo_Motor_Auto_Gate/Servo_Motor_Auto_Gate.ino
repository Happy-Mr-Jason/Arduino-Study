#include <Servo.h>
#define TRIG 3
#define ECHO 2
#define RED 9
#define GREEN 11

Servo myservo;
int pos = 0 ; //angle
String status;
void setup() {
  myservo.attach(6);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
    Ultra_Trigger();
    long distance = pulseIn(ECHO, HIGH) / 58.2;
    Serial.println(String(" distance : ") + distance);
    if(distance < 30){
      gate_open();
    }else{
      gate_close();
    }
}

void gate_open(){
  Serial.println(String("Open"));
  digitalWrite(GREEN,HIGH);
  digitalWrite(RED,LOW);
  if(pos != 90){
    for(int i = 0 ; i < 90 ; i++){
      pos = i;
      myservo.write(pos);  
      delay(15);
    } 
  }
}

void gate_close(){
  Serial.println(String("Close"));
  digitalWrite(RED,HIGH);
  digitalWrite(GREEN,LOW);
  if(pos != 0){
    for(int i = 90 ; i <= 0 ; i--){
    pos = i;
    myservo.write(pos);
    delay(15);  
    }
  }
}

void Ultra_Trigger(){
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(2);
  digitalWrite(TRIG, LOW);
}
