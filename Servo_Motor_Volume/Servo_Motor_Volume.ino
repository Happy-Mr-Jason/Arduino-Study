#include <Servo.h>
Servo myservo;
int pos = 0 ; //angle
int volume = 0;
void setup() {
  myservo.attach(6);
  Serial.begin(9600);
}

void loop() {
    volume = analogRead(A0);
    pos = map(volume,0,1023,0,120);
    myservo.write(pos);
    delay(15);  
    
    Serial.println(String("volume : ") + volume + String(" Position : ") + pos);
}
