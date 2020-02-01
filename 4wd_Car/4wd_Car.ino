#include <SoftwareSerial.h>

#define U1_TRIG 2
#define U1_ECHO 3
#define U2_TRIG 4
#define U2_ECHO 5
#define U3_TRIG 6
#define U3_ECHO 7
#define IN1 8
#define IN2 9
#define IN3 12
#define IN4 13
#define L_SPEED 10
#define R_SPEED 11

void setup()
{
    Serial.begin(9600);
    pinMode(U1_TRIG, OUTPUT);
    pinMode(U1_ECHO, INPUT);
    pinMode(U2_TRIG, OUTPUT);
    pinMode(U2_ECHO, INPUT);
    pinMode(U3_TRIG, OUTPUT);
    pinMode(U3_ECHO, INPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(L_SPEED, OUTPUT);
    pinMode(R_SPEED, OUTPUT);

    stopCar();
}

void loop()
{
    analogWrite(L_SPEED, 255);
    analogWrite(R_SPEED, 255);
    
    runCarforward();
    delay(2000);
    stopCar();
    
    turnCarLeft();
    delay(2000);
    stopCar();
           
}

void stopCar(){
    stopLeft();
    stopRight();
}

void runCarforward(){
    runRightForward();
    runLeftForward();
}

void runCarBackward(){
    runRightBackward();
    runLeftBackward();
}

void turnCarLeft(){
    runRightForward();
}

void turnCarRight(){
    runLeftForward();
}

void returnCarLeft(){
    runRightBackward();
}

void returnCarRight(){
    runLeftBackward();
}

void runLeftForward(){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    Serial.println("Left Forward");
}

void runLeftBackward(){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    Serial.println("Left Backward");
}

void stopLeft(){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    Serial.println("Left Stop");
}

void  runRightForward(){
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Right Forward");
}

void  runRightBackward(){
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Right Backward");
}
void stopRight(){
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    Serial.println("Right Stop");
}