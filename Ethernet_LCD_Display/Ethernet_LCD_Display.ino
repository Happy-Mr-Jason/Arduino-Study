//LCD Display library
#include <LiquidCrystal_I2C.h>
//I2C Communication library
#include <Wire.h>
//Ethernet SPI Communication library
#include <SPI.h>
#include <Ethernet.h>

//LCD I2C Address
#define LCD_ADDRESS 0x27
LiquidCrystal_I2C lcd(LCD_ADDRESS, 16, 2);

//MAC address for Arduino
byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC};
//IP address for Arduino
IPAddress IP(192, 168, 2, 77);
//Create a Web-server
EthernetServer server(80);

String request = "";
void setup()
{
    //Serial Communication Init & begin
    Serial.begin(9600);
    //LCD Init & begin
    lcd.init();
    lcd.backlight();
    //Ethernet Communication Init & begin
    Ethernet.begin(MAC, IP);
    server.begin();
}

void loop()
{
    EthernetClient client = server.available();
    if (client)
    {
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                request += c;
                if (c == '\n')
                {

                    show_main_page(client);
                    delay(1);
                    client.stop();
                    Serial.println(request);
                    String req_back = request;
                    if (request.indexOf("value1") >= 0 && request.indexOf("value2") >= 0)
                    {
                        lcd.clear();
                        //Example request : GET /?value1=textline1&value2=textline2 HTTP/1.1
                        //remove "GET /?value1=" >> length : 13
                        request.remove(0, 13);
                        Serial.println(request);
                        //Ex "textline1&value2=textline2 HTTP/1.1"
                        for (int t = 16; t >= 0; t--)
                        {
                            request.remove(t);
                            if (request.lastIndexOf("&") == -1)
                            {
                                break;
                            }
                        }
                        Serial.print("1'st Line Text:");
                        Serial.println(request);
                        lcd.setCursor(0, 0);
                        lcd.print(request);

                        request = req_back;
                        request.remove(0, 13);
                        int temp = 0;
                        int count = 0;
                        while (1)
                        {
                            request.remove(0, temp);
                            Serial.println(request);
                            if (request.startsWith("&value2"))
                            {
                                Serial.print("Start of 2'nd Line!! ");
                                //&value2=line2text2text22 HTTP/1.1
                                Serial.println(request);
                                req_back = request;
                                for (int i = 16; i < 32; i++)
                                {
                                    request.remove(i);
                                    Serial.print("Remain text:");
                                    Serial.println(request);
                                    if (request.lastIndexOf("HTTP/1.1") != -1)
                                    {
                                        request.remove(i - 8);
                                        break;
                                    }
                                    request = req_back;
                                }
                                //remove "&value2="
                                request.remove(0, 8);
                                Serial.print("2'nd Line Text:");
                                Serial.println(request);
                                lcd.setCursor(0, 1);
                                lcd.print(request);
                                break;
                            }
                            temp = 1;
                            count++;
                            if (request.length() == 0)
                            {
                                Serial.println("Delete lcd");
                                lcd.setCursor(0, 0);
                                lcd.print("");
                                break;
                            }
                        }
                    }
                    request = "";
                }
            }
        }
    }
    // lcd.scrollDisplayLeft();
}

//HTML Main Form
void show_main_page(EthernetClient client)
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection:close");
    client.println();
    client.println("<!DOCTYPE html>");
    client.println("<html lang='kor'>");
    client.println("<head>");
    client.println("<meta charset='UTF-8'>");
    client.println("<title>Arduino Ethernet Server</title>");
    client.println("</head>");
    client.println("<body>");
    client.println("<h1>Wellcome Arduino-Ethernet-Server!!</h1>");
    client.println("<p>아두이노 이더넷 서버에 오신것을 환영합니다.</p>");
    add_html_contents(client);
    client.println("</body>");
    client.println("</html>");
    client.println();
}

//added Html contents
void add_html_contents(EthernetClient client)
{
    client.println("<p>회사 옥외 광고판 내용 변경하기</p>");
    client.println("<form method='GET'>");
    client.println("<p>옥외 광고 윗줄(최대 16자) 내용</p>");
    client.println("<input type='text' name='value1'>");
    client.println("<p>옥외 광고 아랫줄(최대 16자) 내용</p>");
    client.println("<input type='text' name='value2'>");
    client.println("</br>");
    client.println("<input type='submit' value='전송'>");
    client.println("</form>");
}