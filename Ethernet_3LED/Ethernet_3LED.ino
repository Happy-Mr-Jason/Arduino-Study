#include <SPI.h>
#include <Ethernet.h>

//MAC address for Arduino
byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC};
//IP address for Arduino
IPAddress IP(192, 168, 2, 77);
//Create a Web-server
EthernetServer server(80);

//클라이언트가 웹서버에 접속하면서 웹 브라우저가 웹서버로 처음 보내는 문자열
String HTTP_req = "";
int led_pin = 9;
int sel_LED = 0;

int LED_R = A0;
int LED_G = A1;
int LED_Y = A2;

String r_checked = "";
String g_checked = "";
String y_checked = "";

int led_r_status = LOW;
int led_g_status = LOW;
int led_y_status = LOW;

void setup()
{
    Serial.begin(9600);
    // dht.begin();
    Ethernet.begin(MAC, IP);
    server.begin();
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_Y, OUTPUT);

    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_Y, LOW);
}

void loop()
{
    ethernetClients();
}

void ethernetClients()
{
    EthernetClient client = server.available();
    if (client)
    {
        Serial.println("Clients are connected!! Send the masseges!!");
        boolean current = true;
        //clients are connected to server
        while (client.connected())
        {
            if (client.available())
            {
                //first character from client
                char c = client.read();
                HTTP_req += c;

                if (c == '\n' && current)
                {
                    //transfer the HTML source to client
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type : text/html");
                    client.println("Connection close");
                    // client.println("Refresh : 5");
                    client.println();
                    client.println("<!DOCTYPE html>");
                    client.println("<html lang='kor'>");
                    client.println("<head>");
                    client.println("<meta charset='UTF-8'>");
                    client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
                    client.println("<meta http-equiv='X-UA-Compatible' content='ie=edge'>");
                    client.println("<title>Arduino Ethernet Server</title>");

                    client.println("<style>");
                    client.println("body p{color:Blue; text-align:center;}");
                    client.println("</style>");

                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>Wellcome Arduino-Ethernet-Server!!</h1>");
                    client.println("<p>아두이노 이더넷 서버에 오신것을 환영합니다.</p>");

                    client.println("<p>원하는 색상을 선택하세요</p></br>");
                    client.println("<form method='GET'>");
                    ProcessCheck(client);
                    client.println("</form>");

                    client.println("</body>");
                    client.println("</html>");
                    //print the data from client

                    Serial.println(String("클라이언트에서 받은 내용 \n"));
                    Serial.println(HTTP_req);
                    HTTP_req = "";
                }
                if (c == '\n')
                {
                    current = true;
                }
                else if (c != '\r')
                {
                    current = false;
                }
            }
        }
        delay(1);
        client.stop();
    }
}

void ProcessCheck(EthernetClient client)
{
    Serial.println(String("received \n") + HTTP_req);
    //클라이언트에서 보낸 메시지 (기본 메시지 + Submit으로 보내온 메시지)에 "LED=1"이 있는지 찾기
    if (HTTP_req.indexOf("sel_LED=1") > -1)
    {
        sel_LED = 1;
    }
    if (HTTP_req.indexOf("sel_LED=2") > -1)
    {
        sel_LED = 2;
    }
    if (HTTP_req.indexOf("sel_LED=3") > -1)
    {
        sel_LED = 3;
    }

    switch (sel_LED)
    {
    case 1:
        //Check LED LED_status
        led_r_status = HIGH;
        led_g_status = LOW;
        led_y_status = LOW;
        r_checked = "checked";
        g_checked = " ";
        y_checked = " ";
        break;
    case 2:
        //Check LED LED_status
        led_r_status = LOW;
        led_g_status = HIGH;
        led_y_status = LOW;
        r_checked = " ";
        g_checked = "checked";
        y_checked = " ";
        break;
    case 3:
        //Check LED LED_status
        led_r_status = LOW;
        led_g_status = LOW;
        led_y_status = HIGH;
        r_checked = " ";
        g_checked = " ";
        y_checked = "checked";
        break;
    default:
        led_r_status = HIGH;
        led_g_status = LOW;
        led_y_status = LOW;
        r_checked = "checked";
        g_checked = " ";
        y_checked = " ";
        break;
    }

    digitalWrite(LED_R, led_r_status);
    digitalWrite(LED_G, led_g_status);
    digitalWrite(LED_Y, led_y_status);

    client.print("<input type='radio' name='sel_LED' value='1' onclick='submit();'");
    client.print(String(r_checked));
    client.println("'/> 빨강");
    client.print("<input type='radio' name='sel_LED' value='2' onclick='submit();'");
    client.print(String(g_checked));
    client.println("'/> 녹색");
    client.print("<input type='radio' name='sel_LED' value='3' onclick='submit();'");
    client.print(String(y_checked));
    client.println("'/> 노랑");
    Serial.println(String("R : " + r_checked) + " " + String("G : " + g_checked) + " " + String("Y : " + y_checked));
}
