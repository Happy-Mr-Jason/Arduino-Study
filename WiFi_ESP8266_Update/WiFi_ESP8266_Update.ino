#include <SoftwareSerial.h>
#define ARD_RX_ESP_TX 6
#define ARD_TX_ESP_RX 7
SoftwareSerial WiFiSerial(ARD_RX_ESP_TX, ARD_TX_ESP_RX);
/*
AT
//Check Firmware Version
AT+GMR
//Check ESP Module Mode 1 / Station 2 AP 3 Both
AT+CWMODE_DEF=3
//WiFi Connect Check
AT+CWJAP="SJ_WiFi","73077307"
//Got IP Address
AT+CIPSTA_CUR?
//Change Baud rate and save
AT+UART_DEF=115200,8,1,0,1   =>   AT+UART_DEF=9600,8,1,0,1
*/

//SDK Verson 2.2.1로 성공 2020-02-16 기준
//9600baud 로 바꾼후 AT 명령이 깔끔하게 나오면 성공
//WiFi_ESP8266_Check로 AP 접속 까지 테스트 진행 할 것

void setup()
{
    Serial.begin(9600);  // 초기에는 115200
    WiFiSerial.begin(9600); // 초기에는 115200 으로 설정하고 AT+UART_DEF=9600,8,1,0,0 명령으로 9600으로 낮춘다.
}

void loop()
{
    if(Serial.available()){
        char c = Serial.read();
        WiFiSerial.write(c);
    }

    if(WiFiSerial.available()){
        char c = WiFiSerial.read();
        Serial.write(c);
    }
}