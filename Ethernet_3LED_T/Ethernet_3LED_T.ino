#include <SPI.h>
#include <Ethernet.h>

//MAC address for Arduino
byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC};
//IP address for Arduino
IPAddress IP(192, 168, 2, 77);
//Create a Web-server
EthernetServer server(80);

int LED_R = 4;
int LED_G = 5;
int LED_B = 6;

void setup()
{
    Serial.begin(9600);
    // dht.begin();
    Ethernet.begin(MAC, IP);
    server.begin();
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
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
        String buffer = "";
        //clients are connected to server
        while (client.connected())
        {
            if (client.available())
            {
                char c = client.read();
                buffer += c;
                if (c == '\n')
                {
                    buffer = "";
                }
                else if (c == '\r')
                {
                    Serial.println(buffer);
                    if (buffer.indexOf("GET /led.cgi?led_status=1") >= 0)
                    {
                        digitalWrite(LED_R, HIGH);
                        digitalWrite(LED_G, LOW);
                        digitalWrite(LED_B, LOW);
                        status_check(client);
                    }
                    else if (buffer.indexOf("GET /led.cgi?led_status=2") >= 0)
                    {
                        digitalWrite(LED_R, LOW);
                        digitalWrite(LED_G, HIGH);
                        digitalWrite(LED_B, LOW);
                        status_check(client);
                    }
                    else if (buffer.indexOf("GET /led.cgi?led_status=3") >= 0)
                    {
                        digitalWrite(LED_R, LOW);
                        digitalWrite(LED_G, LOW);
                        digitalWrite(LED_B, HIGH);
                        status_check(client);
                    }
                    else
                    {
                        // digitalWrite(LED_R, LOW);
                        // digitalWrite(LED_G, LOW);
                        // digitalWrite(LED_B, LOW);
                        status_check(client);
                    }
                }
            }
        }
    }
}

void status_check(EthernetClient client)
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
    if (digitalRead(LED_R) > 0)
    {
        client.println("LED가 현재 <span style='color:red; font-weight:bold;'> RED </span> 상태입니다.");
    }
    else if (digitalRead(LED_G) > 0)
    {
        client.println("LED가 현재 <span style='color:green; font-weight:bold;'> GREEN </span> 상태입니다.");
    }
    else if (digitalRead(LED_B) > 0)
    {
        client.println("LED가 현재 <span style='color:blue; font-weight:bold;'> BLUE </span> 상태입니다.");
    }
    else
    {
        client.println("LED가 현재 <span style='color:gray; font-weight:bold;'> OFF </span> 상태입니다.");
    }
    client.println("</br>");
    // CGI(Common Gateway Interface)
    client.println("<form method='GET' name='led_status' action='/led.cgi'>");
    radioHTML(client);
    client.println("<input type='submit' value='전송'/>");
    client.println("</form>");
    client.println("</body>");
    client.println("</html>");
    client.stop();
}

void radioHTML(EthernetClient client)
{
    if (digitalRead(LED_R) > 0)
    {
        client.println("<p><input type='radio' name='led_status' value='1' checked/>RED</p>");
        client.println("<p><input type='radio' name='led_status' value='2'/>GREEN</p>");
        client.println("<p><input type='radio' name='led_status' value='3'/>BLUE</p>");
    }
    else if (digitalRead(LED_G) > 0)
    {
        client.println("<p><input type='radio' name='led_status' value='1'/>RED</p>");
        client.println("<p><input type='radio' name='led_status' value='2' checked/>GREEN</p>");
        client.println("<p><input type='radio' name='led_status' value='3'/>BLUE</p>");
    }
    else if (digitalRead(LED_B) > 0)
    {
        client.println("<p><input type='radio' name='led_status' value='1'/>RED</p>");
        client.println("<p><input type='radio' name='led_status' value='2'/>GREEN</p>");
        client.println("<p><input type='radio' name='led_status' value='3' checked/>BLUE</p>");
    }
    else
    {
        client.println("<p><input type='radio' name='led_status' value='1'/>RED</p>");
        client.println("<p><input type='radio' name='led_status' value='2'/>GREEN</p>");
        client.println("<p><input type='radio' name='led_status' value='3'/>BLUE</p>");
    }
    
}
