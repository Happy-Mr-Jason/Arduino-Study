#include <Servo.h>
Servo myservo;
int pos = 0 ; //angle
void setup() {
  myservo.attach(6);
}

void loop() {
  for(pos = 0 ; pos <150 ; pos++){
    myservo.write(pos);
    delay(15);
  }
}
