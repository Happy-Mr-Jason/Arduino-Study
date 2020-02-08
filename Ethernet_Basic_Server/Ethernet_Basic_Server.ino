#include <Ethernet.h>
#include <SPI.h>

//MAC address for Arduino
byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC};
//IP address for Arduino
IPAddress IP(192, 168, 2, 77);
// Create a Web-server 
EthernetServer server(80);

void setup()
{
    Serial.begin(9600);
    //Ethernet Sheild init and start
    Ethernet.begin(MAC, IP);
    //Web-server start
    server.begin();
    delay(2000);

    print_address(String("IP Address : "), Ethernet.localIP());
    print_address(String("Subnet Mask : "), Ethernet.subnetMask());
    print_address(String("Gatway Address : "), Ethernet.gatewayIP());
    print_address(String("DNS Address : "), Ethernet.dnsServerIP());
}

void loop()
{
    // a client connect to server....
    EthernetClient client = server.available();
    if (client)
    {
        Serial.println("Clients are connected!! Send the masseges!!");
        boolean current = true;
        while (client.available())
        {
            char c = client.read();
            if (current)
            {
                //transfer the HTML source to client
                client.println("HTTP/1.1 200 OK");
                client.println("Content-Type : text/html");
                client.println("Connection close");
                //Refresh interval 3sec
                client.println("Refresh: 3");
                client.println();
                client.println("<!DOCTYPE html>");
                client.println("<html lang='kor'>");
                client.println("<head>");
                client.println("<meta charset='UTF-8'>");
                client.println("<meta name='viewport' content='width=device-width, initial-scale=1.0'>");
                client.println("<meta http-equiv='X-UA-Compatible' content='ie=edge'>");
                client.println("<title>Arduino Ethernet Server</title>");
                client.println("</head>");
                client.println("<body>");
                client.println("<h1>Wellcome Arduino-Ethernet-Server!!</h1>");
                client.println("<p>아두이노 이더넷 서버에 오신것을 환영합니다.</p>");
                client.println("<p>이제부터 이더넷을 통한 제어를 시작합니다.</p>");
                client.print("<span>Resistans Volume Value : <span>");
                client.println(analogRead(A0));
                client.println("</br>");
                client.println("</body>");
                client.println("</html>");
                if (c != '\r')
                {
                    current = false;
                }
            }
        }
        delay(1);
        client.stop();
    }
}

void print_address(String header, IPAddress address)
{
    Serial.print(header);
    for (int i = 0; i < 4; i++)
    {
        Serial.print(address[i], DEC);
        if (i != 3)
        {
            Serial.print(".");
        }
    }
    Serial.println("");
}