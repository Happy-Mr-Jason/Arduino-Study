#include <DHT.h>
#include <SPI.h>
#include <Ethernet.h>

#define DHTPIN 7
#define DHTTYPE DHT11
// Create DHT Object
DHT dht(DHTPIN, DHTTYPE);

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

int LED_R = A2;
int LED_G = A1;
int LED_Y = A0;

float h;
float t;
float f;
float hif;
float hic;

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
    h = dht.readHumidity();
    t = dht.readTemperature();
    f = dht.readTemperature(true);

    if (isnan(h) || isnan(t) || isnan(f))
    {
        Serial.println(String("Error is ocured on DHT Sensor......"));
    }

    hif = dht.computeHeatIndex(f, h);        // Compensated 'F
    hic = dht.computeHeatIndex(t, h, false); // Compensated 'C

    Serial.print(String("Humidity : ") + h);
    Serial.print(String(" Temperature(C) : ") + t);
    Serial.print(String(" Temperature(F) : ") + f);
    Serial.print(String(" ComputedTemperature(C) : ") + hic);
    Serial.println(String(" ComputedTemperature(F) : ") + hif);

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

                    client.println("<p>========================================</p>");
                    client.print("<p>현재 우리집 온도 : ");
                    client.print(hic);
                    client.println("도</p>");
                    client.println("<p>========================================</p>");
                    client.print("<p>현재 우리집 습도 : ");
                    client.print(h);
                    client.println("%</p>");
                    client.println("<p>========================================</p>");
                    client.println("</br>");

                    client.println("</body>");
                    client.println("</html>");
                    //print the data from client

                    Serial.println(String("클라이언트에서 받은 내용 \n"));
                    Serial.println(HTTP_req);
                    break;
                }
                if (c == '\n')
                {
                    current = true;
                    HTTP_req = "";
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

