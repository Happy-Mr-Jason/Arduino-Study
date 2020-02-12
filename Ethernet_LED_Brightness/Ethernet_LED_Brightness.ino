#include <SPI.h>
#include <Ethernet.h>

//MAC address for Arduino
byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC};
//IP address for Arduino
IPAddress IP(192, 168, 2, 77);
//Create a Web-server
EthernetServer server(80);

int LED = 3;
int brightness = 0;
String buffer = "";
String valueString;

void setup()
{
    Serial.begin(9600);
    Ethernet.begin(MAC, IP);
    server.begin();
    delay(2000);
    pinMode(LED, OUTPUT);
    analogWrite(LED, brightness);
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
        //Client(Webbrowser) is connected to server(Arduino)
        int response_type = -1;
        while (client.connected())
        {
            //Receive data per a Line
            //If received data is empty line
            boolean empty_line = read_one_line(client);
            Serial.println(buffer);
            
            int pos = buffer.indexOf("GET /led.cgi?brightness=");
            if (pos >= 0)
            {
                valueString = "";
                //add pos length of "GET /led.cgi?brightness="
                pos += 24;
                int pos_end = -1;
                for (int i = pos; i < buffer.length(); i++)
                {
                    if (buffer[i] == ' ')
                    {
                        //find space
                        pos_end = i;
                        break;
                    }
                }
                if (pos_end != -1)
                {
                    valueString = buffer.substring(pos, pos_end);
                }
            }
            if (empty_line)
                break; // Finished receive data(HTML)
        }
        int input = valueString.toInt();
        if (input >= 0 && input <= 255)
        {
            brightness = input;
            analogWrite(LED, brightness);
        }
        show_main_page(client);
        client.stop();
    }
}

boolean read_one_line(EthernetClient client)
{
    buffer = "";
    while (client.available())
    {
        //Read a byte data
        char c = client.read();
        //\r :
        if (c == '\r')
        {
            client.read(); // ignore '\n' character, HTML has \r\n on end of the each file
            break;
        }
        buffer += c;
    }
    return (buffer.length() == 0);
}

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
    client.print("LED 밝기(0~255) : ");
    client.println(brightness);
    client.println("</br>");
    client.println("<form method='GET' action='/led.cgi'>");
    // client.println("<p>밝기 : <input type='text' name='brightness'/></p>");
    client.println("<p>밝기 : <input type='range' name='brightness' min='0' max='255' /></p>");
    client.println("<input type='submit' value='전송'/>");
    client.println("</form>");
    client.println("</body>");
    client.println("</html>");
    client.stop();
}
