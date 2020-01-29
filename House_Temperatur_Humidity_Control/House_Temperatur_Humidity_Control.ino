#include <DHT.h>               //온습도계 라이브러리
#include <LiquidCrystal_I2C.h> // LCD I2C모듈(MH)용 라이브러리
#include <Wire.h>              // I2C 모듈 주소를 찾기 위한 라이브러리
#include <Servo.h>             //서보모터 라이브러리

DHT dht(A0, DHT11);                 //온습도계 DHT의 객체 생성 A0:입력핀 / DHT11:센서 타입
LiquidCrystal_I2C lcd(0x27, 16, 2); //LCD 객체 생성 0x27 : I2C address / 16 : 행수 / 2 : 열수
Servo servo;                        // 서보 모터 객체 생성

float mTemperature;    //온도값 변수
float mHumidity;       //습도값 변수
int mAngle = 0;        //서보모터 회전 각도
float mSetHumidity;    //설정 습도값 변수
String mLine0, mLine1; //LCD에 표시할 문자열 저장 변수

//프로그램 초기화 및 설정
void setup()
{
    dht.begin();
    lcd.init();
    lcd.backlight();
    servo.attach(9);
    Serial.begin(9600);
}

//메인 루프 프로그램
void loop()
{
    mTemperature = dht.readTemperature();                //온도 읽어 오기
    mHumidity = dht.readHumidity();                      // 습도 읽어 오기
    mSetHumidity = map(analogRead(A1), 0, 1024, 0, 100); //A1(가변저항) 으로 설정값 얻어오기
    mLine0 = String("Amb' : ") + int(mTemperature) + String("C ") + int(mHumidity) + String("%");
    Serial.println(mLine0);
    mLine1 = String("Set' : ") + int(mSetHumidity) + String("%");
    Serial.println(mLine1);
    lcd_Display();
    if (int(mHumidity) < int(mSetHumidity))
    {
        sv_Open(); // 환기구 열기
    }
    else
    {
        sv_Close(); // 환기구 닫기
    }
}

//LCD Display
void lcd_Display()
{
    lcd.setCursor(0, 0);
    lcd.print(mLine0);
    lcd.setCursor(0, 1);
    lcd.print(mLine1);
}

//열림 : 서보모터 90도 열림
void sv_Open()
{
    Serial.println(String("Open") + mAngle);
    if (mAngle != 90)
    {
        for (int i = 0; i <= 90; i++)
        {
            mAngle = i;
            servo.write(mAngle);
            delay(15);
        }
        Serial.println(String("angle" + mAngle));
    }
}

//닫힘 : 서보모터 0도 복귀
void sv_Close()
{
    Serial.println(String("Close") + mAngle);
    if (mAngle != 0)
    {
        for (int i = 90; i >= 0; i--)
        {
            mAngle = i;
            servo.write(mAngle);
            delay(15);
        }
        Serial.println(String("angle" + mAngle));
    }
}
