#include <LiquidCrystal.h>
#define ECHO 8
#define TRIG 7
#define LED 6

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // LCD 초기화(생성)
long distance = 0;
void setup()
{
    pinMode(ECHO, INPUT);
    pinMode(TRIG, OUTPUT);
    pinMode(LED, OUTPUT);
    lcd.begin(16, 2);          //16열 2행
    lcd.print("hello world!"); //첫행에 표시
    Serial.begin(9600);
}

void loop()
{
    measure_distance();
    lcd.clear();
    if(distance <= 10){
        lcd.setCursor(0,0);
        lcd.print("warning");
        lcd.setCursor(0,1);
        lcd.print(distance + String("cm"));
        digitalWrite(LED, HIGH);
    } else {
        lcd.setCursor(0,0);
        lcd.print("safe");
        lcd.setCursor(0,1);
        lcd.print(distance + String("cm"));
        digitalWrite(LED, LOW);
    }
    delay(200);    
}

void measure_distance(){
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(2);
    digitalWrite(TRIG, LOW);
    distance = pulseIn(ECHO, HIGH) / 58.2;
    //Serial.println(distance + "");
}
