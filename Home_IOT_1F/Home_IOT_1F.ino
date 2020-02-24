#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

//변수 선언부
#define SLAVE 1
SoftwareSerial BTSerial(2, 3);
byte bufferthree[4]; //데이터를 수신하는 수신버퍼

LiquidCrystal_I2C lcd(0x27, 16, 2); //I2C주소를 가지고있는 lcd객체 생성
#define RST_PIN 9
#define SS_PIN 10
Servo mSV_Front_Door;
Servo mSV_Park_Zone;
//초음파 센서 중 TRIG
#define TRIG 5
//초음파 센서 중 ECHO
#define ECHO 6
//초음파 센서 거리 변수
long distance = 0;

int blue = A0;
int green = A1;
int red = A2;
int buzer = 4;
int gasPin = A3;
boolean closeDoor = false;

byte Access[] = {231, 0, 227, 25}; //등록할 카드 번호 배열 선언
boolean cCard;                     //등록된 카드 확인하기 위한 변수 선언

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance 객체 생성
int val = 90;
String buffertwo = ""; //2
//byte index[4];                                //
uint8_t sendData2[2]; //2바이트 형 상태 전송용 버퍼
uint8_t recvData2[2]; //2바이트 형  명령 수신용 버퍼
uint8_t sendData4[4]; //4바이트 형 상태 전송용 버퍼
uint8_t recvData4[4]; //4바이트 형  명령 수신용 버퍼

//time interval
long started_time = 0;
long current_time = 0;
long interval_time = 5000000; // 20sec

//timer for 1sec
long started_time_1sec = 0;
long current_time_1sec = 0;
long interval_time_1sec = 1000000; // 1sec

//timer for 1sec
long started_time_3sec = 0;
long current_time_3sec = 0;
long interval_time_3sec = 3000000; // 3sec

//변수 선언부 끝

void setup()
{
    Wire.begin(1);
    Serial.begin(9600); // Initialize serial communications with the PC
    BTSerial.begin(9600);
    SPI.begin();              // Init SPI bus  초기화
    mfrc522.PCD_Init();       // Init MFRC522 card
    pinMode(red, OUTPUT);     //RGB  LED RED
    pinMode(green, OUTPUT);   //RGB LED green
    pinMode(blue, OUTPUT);    //RGB LED BLUE
    pinMode(TRIG, OUTPUT);    //초음파 TRIG
    pinMode(ECHO, INPUT);     //초음파 ECHO
    analogWrite(red, 0);      //RGB LED 초기값(0)
    analogWrite(green, 0);    //RGB LED 초기값(0)
    analogWrite(blue, 0);     //RGB LED 초기값(0)
    mSV_Front_Door.attach(8); //현관문 모터 D8
    mSV_Park_Zone.attach(7);  //주차장문 모터 D7
    mSV_Front_Door.write(0);  //현관문 모터 초기값 (닫힘)
    mSV_Park_Zone.write(90); //현관문 모터 초기값 (닫힘) --> 모터의 각도설정상 반대로 설정
    lcd.init();               // I2C LCD를 초기화
    lcd.backlight();          // I2C LCD백라이트를 켜줍니다.
}

void loop()
{
    thdRead();       //온 습도 센서(2층)
    bluetoothRead(); //블루투스 데이터 송신 함수(1,2층)
    parking_Zone();  //초음파 센서 동작 함수(1층 주차장)
    gas_Pin();       //가스 센서 함수(1층)
    homedataRead();  //홈 데이터 전송 함수(1,2층)
    rfCardDoor();    //rf카드 동작 함수(1층 현관문)
}

void bluetoothRead()
{
    if (BTSerial.available())
    { // 블루투스로 데이터를 읽었을때...
        BTSerial.readBytes(recvData4, 4);
        Serial.print("Recieved Data from Android : ");
        Serial.print(recvData4[0], HEX);
        Serial.print(" ");
        Serial.print(recvData4[1], HEX);
        Serial.print(" ");
        Serial.print(recvData4[2], HEX);
        Serial.print(" ");
        Serial.println(recvData4[3], HEX);

        if (recvData4[0] < 150)
        { // 블루투스로 읽은 데이터를 rcvByte에 넘겨줌.

            if (recvData4[0] == 51 && recvData4[1] == 0)
            {
                mSV_Front_Door.write(val);
            }
            else if (recvData4[0] == 51 && recvData4[1] == 1)
            {
                mSV_Front_Door.write(0);
            }
            else if (recvData4[0] == 50 && recvData4[1] == 0)
            {
                parking_zone_close();
            }
            else if (recvData4[0] == 50 && recvData4[1] == 1)
            {
                parking_zone_open();
            }
        }
        else if (recvData4[0] >= 150)
        {
            Wire.beginTransmission(1); // 1번 슬레이브와 통신 시작

            Wire.write(recvData4[0]);
            Wire.write(recvData4[1]);
            Wire.write(recvData4[2]);
            Wire.write(recvData4[3]);

            // sendData4w(recvData4[0], recvData4[1], recvData4[2], recvData4[3]);
            Wire.endTransmission(1); // 전송 종료
        }
        buffertwo = "";
    }
}

void thdRead()
{
    current_time = micros();
    //20초 마다 실행 하는 곳  interval_time  = 20000000 (20sec)
    if (started_time + interval_time <= current_time)
    {
        tempHumDust();
        Serial.println(started_time);
        started_time = current_time;
    }
}
void tempHumDust()
{

    int temperature;
    int moisture;
    int dust;

    Wire.requestFrom(1, 4);
    if (Wire.available() == 4)
    { //

        for (int i = 0; i < 4; i++)
        {
            bufferthree[i] = Wire.read(); // gets the data
        }
        Serial.print("Recieved Data From 2'nd Floor : ");
        Serial.print(bufferthree[0]);
        Serial.print(" ");
        Serial.print(bufferthree[1]);
        Serial.print(" ");
        Serial.print(bufferthree[2]);
        Serial.print(" ");
        Serial.println(bufferthree[3]);

        if (bufferthree[0] < 150)
        {
            //온 습 먼지
            // temperature = bufferthree[0];
            // moisture = bufferthree[1];
            // dust = bufferthree[2];
            // sendData2w(100, temperature);
            // sendData2w(101, moisture);
            // sendData2w(102, dust);
            // Serial.print(": Temperature = ");
            // Serial.print(temperature);
            // Serial.print("; Moisture = ");
            // Serial.println(moisture);
            // Serial.print("; dust = ");
            // Serial.println(dust);
        }
        else if (bufferthree[0] >= 150)
        {
            sendData4w(bufferthree[0], bufferthree[1], bufferthree[2], bufferthree[3]);
        }
    }
}

void sendData2w(uint8_t a, uint8_t b)
{
    Serial.println("sendData2W Running....");
    BTSerial.write(a);
    BTSerial.write(b);
    BTSerial.write(sendData2, 2);
    BTSerial.write('\n');
    Serial.print((int)sendData2[0]);
    Serial.println((int)sendData2[1]);
}

void sendData4w(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    Serial.println("sendData4W Running....");
    if (a > 128)
    {
        a -= 256;
    }
    if (b > 128)
    {
        b -= 256;
    }
    if (c > 128)
    {
        c -= 256;
    }
    if (d > 128)
    {
        d -= 256;
    }

    Serial.print("send to android : ");
    Serial.print((byte)a);
    Serial.print(" ");
    Serial.print((byte)b);
    Serial.print(" ");
    Serial.print((byte)c);
    Serial.print(" ");
    Serial.println((byte)d);

    BTSerial.write((byte)a);
    BTSerial.write((byte)b);
    BTSerial.write((byte)c);
    BTSerial.write((byte)d);
}

void homedataRead()
{
    int mSV_Front_DoorRead = mSV_Front_Door.read();
    int mSV_Park_ZoneRead = mSV_Park_Zone.read();
    int gasresult = analogRead(gasPin);
    if (mSV_Front_DoorRead >= 20)
    { //현관문 각도가 20도 이상이면
        sendData2w(1, 1);
    }
    else if (mSV_Park_ZoneRead <= 20)
    { //주차장 각도가 20도 이상이면
        sendData2w(0, 1);
    }
    else if (gasresult > 200)
    { //가스측정값이 200이 넘으면
        sendData2w(3, 1);
    }
    else if (mSV_Front_DoorRead < 20)
    { //현관문 각도가 20도 미만이면
        sendData2w(1, 0);
    }
    else if (mSV_Park_ZoneRead > 20)
    { //주차장 각도가 20도 미만
        sendData2w(0, 0);
    }
}

void rfCardDoor()
{
    // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
    if (!mfrc522.PICC_IsNewCardPresent())
    { //테그접촉이 되지 않았을 경우(읽기불가)
        return;
    }
    // Select one of the cards  //카드가 인식 못했을 때
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }
    cCard = true;
    for (byte i = 0; i < 4; i++)
    { //카드 번호가 4자리입니다
        if (mfrc522.uid.uidByte[i] != Access[i])
        { //카드번호 4자리가 같지않으면 수행
            cCard = false;
        }
    }
    if (cCard == true)
    { //등록된 카드면 수행
        sendData2w(2, 0);
        lcd.setCursor(0, 0);
        lcd.print("Ok.Open Door.");
        analogWrite(red, 0);
        analogWrite(green, 0);
        analogWrite(blue, 255);
        front_door_open();
        tone(4, 523, 100);
        delay(1000);
        front_door_close();
    }
    else
    { //등록된 카드가 아니면 수행
        sendData2w(2, 1);
        lcd.setCursor(0, 0);
        lcd.print("Not registered.");
        analogWrite(red, 255);
        analogWrite(green, 0);
        analogWrite(blue, 0);
        front_door_close();
        tone(4, 523, 100);
        //  delay(300);
        // tone(4, 523, 100);
        //   delay(300);
    }
}

void front_door_open()
{
    mSV_Front_Door.write(90);
}

void front_door_close()
{
    mSV_Front_Door.write(0);
}

void parking_Zone()
{
    current_time_1sec = micros();
    //1초 마다 실행 하는 곳  interval_time_1sec  = 1000000 (1sec)
    if (started_time_1sec + interval_time_1sec <= current_time_1sec)
    {
        measure_distance();
        started_time_1sec = current_time_1sec;
    }

    Serial.print("Parking Zone Distance : ");
    Serial.println(distance);

    lcd.clear();
    if (distance < 10 && distance > 5)
    {
        if (closeDoor)
        {
            lcd.setCursor(0, 0);
            lcd.print("Open Door.");
            analogWrite(red, 255);
            analogWrite(green, 0);
            analogWrite(blue, 0);
            parking_zone_open();
            closeDoor = false;
            started_time_3sec = micros();  // 열린 시점 저장
        }
    }
    else
    {
        if (!closeDoor)
        {
            current_time_3sec = micros();
            //3초 후 실행 하는 곳  interval_time  = 20000000 (20sec)
            if (started_time_3sec + interval_time_3sec <= current_time_3sec)
            {

                lcd.setCursor(0, 0);
                lcd.print("Close Door.");
                analogWrite(red, 0);
                analogWrite(green, 0);
                analogWrite(blue, 0);
                parking_zone_close();
                closeDoor = true;

                started_time = current_time_3sec;
            }
        }
    }
}

void measure_distance()
{
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(2);
    digitalWrite(TRIG, LOW);
    distance = pulseIn(ECHO, HIGH) / 58.2;
    // if (digitalRead(ECHO) == LOW)
    // {
    // }
    // else
    // {
    //     pinMode(ECHO, OUTPUT);
    //     digitalWrite(ECHO, LOW);
    //     pinMode(ECHO, INPUT);
    // }
}

void parking_zone_open()
{
    mSV_Park_Zone.write(0);
}

void parking_zone_close()
{
    delay(3000);
    mSV_Park_Zone.write(90);
}

void gas_Pin()
{
    delay(1000);
    int gaspinnum = analogRead(gasPin);
    if (gaspinnum > 200)
    {
        tone(4, 523, 100);
        delay(300);
    }
}
