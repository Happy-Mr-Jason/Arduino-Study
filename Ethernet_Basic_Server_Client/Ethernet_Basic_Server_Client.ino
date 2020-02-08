#include <Ethernet.h>
#include <SPI.h>

//MAC address for Arduino
byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC};
//IP address for Arduino
IPAddress IP(192, 168, 2, 77);
//Create a Web-server
EthernetServer server(80);

//클라이언트가 웹서버에 접속하면서 웹 브라우저가 웹서버로 처음 보내는 문자열
String HTTP_req = "";
int led_pin = 9;
boolean LED_status = 0;

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
          client.println("<h1>LED Control</h1>");
          client.println("<p>LED Switch(On/Off)</p>");
          client.println("<form method='get'>");
          ProcessCheck(client);
          client.println("</form>");
          client.println("</br>");
          client.print("<span>Resistans Volume Value : <span>");
          client.println(analogRead(A0));
          client.println("</br>");
          client.println("</body>");
          client.println("</html>");
          //print the data from client
          Serial.println(HTTP_req);
          HTTP_req = "";
          break;
        }
        if (c == '\n') {
          current = true;
        } else if (c != '\r')
        {
          current = false;
        }
      }
    }
    delay(1);
    client.stop();
  }
}

//
void ProcessCheck(EthernetClient client) {
  //클라이언트에서 보낸 메시지 (기본 메시지 + Submit으로 보내온 메시지)에 "LED=1"이 있는지 찾기
  if (HTTP_req.indexOf("LED=1") > -1) {
    LED_status = 1;
  }
  if (HTTP_req.indexOf("LED=0") > -1) {
    LED_status = 0;
  }

  if (LED_status) {
    //Check LED LED_status
    digitalWrite(led_pin, HIGH);
    client.println("<input type='checkbox' name='LED' value='0' checked onclick='submit();'/> LED LAMP");
  } else {
    digitalWrite(led_pin, LOW);
    client.println("<input type='checkbox' name='LED' value='1' onclick='submit();'/> LED LAMP");
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
