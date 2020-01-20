#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // LCD 초기화(생성)

void setup()
{
    lcd.begin(16,2); //16열 2행 
    lcd.print("hello world!"); //첫행에 표시
    Serial.begin(9600);
}

void loop()
{
    lcd.setCursor(0,1);  // 2번째 행에 커서 표시
    lcd.print(analogRead(A0));
    Serial.println(analogRead(A0));
    delay(200);
}