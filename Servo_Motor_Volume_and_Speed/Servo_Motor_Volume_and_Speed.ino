#include <Servo.h>
Servo myservo;
#define VOL A0
#define S_VOL A1

int pos = 0 ; //angle
int speed = 0; //speed
void setup() {
  myservo.attach(6);
  Serial.begin(9600);
}

void loop() {
  pos = map(analogRead(VOL), 0, 1024, 0, 180);
  speed = map(analogRead(S_VOL), 0, 1024, 60, 0);
  Serial.println(String("Servo Position : ") + myservo.read() + String(" Target Position : ") + pos + String(" Speed : ") + speed + String(" msec/angle"));
  if (myservo.read() < pos) {
    Serial.println(String("Turnning Positive..."));
    for (int i = myservo.read(); i <= pos ; i++) {
      myservo.write(i);
      delay(speed);
    }
  } else if (myservo.read() > pos) {
    Serial.println(String("Turn Negativ..."));
    for (int i = myservo.read(); i >= pos ; i--) {
      myservo.write(i);
      delay(speed);
    }
  } else {
    Serial.println(String("Hold..."));
  }
}
