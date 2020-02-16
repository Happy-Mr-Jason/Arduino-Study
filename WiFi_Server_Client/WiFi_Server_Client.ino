#include "WiFiEsp.h"
// Emulate Serial1 on pins 7/6 if not present
// #ifndef HAVE_HWSERIAL1   //Program will Start when Serial Monitor available.
#include "SoftwareSerial.h"
#define ARD_RX_ESP_TX 6
#define ARD_TX_ESP_RX 7
SoftwareSerial WiFiSerial(ARD_RX_ESP_TX, ARD_TX_ESP_RX);
// #endif

char ssid[] = "SJ_WiFi";  // network SSID (name)
char pass[] = "73077307"; // network password

IPAddress ipaddress;
WiFiEspServer server(80);

const int IDLE_SATATUS = 0;
const int TRY_CONNECT_SSID = 1;
const int CONNECTED_SSID = 2;
const int GOT_IPADDRESS = 3;
const int SERVER_STARTED = 4;
/* IDLE_SATATUS
TRY_CONNECT_SSID
CONNECTED_SSID
GOT_IPADDRESS 
SERVER_STARTED */
int status = IDLE_SATATUS;
int refresh_cnt = 0;

String read_buffer = "";
void setup()
{
    Serial.begin(9600);
    WiFiSerial.begin(9600);
    WiFi.init(&WiFiSerial);
}

void loop()
{
    while (status < CONNECTED_SSID)
    {
        Serial.print("Attempting to connect to WPA SSID: ");
        Serial.println(ssid);
        int beginStatus = WiFi.begin(ssid, pass);
        Serial.println(beginStatus, DEC);
        status = TRY_CONNECT_SSID;
        if (beginStatus == 1)
        {
            status = CONNECTED_SSID;
        }
        delay(5000);
    }

    wifi_Status_Check();
    delay(1000);

    WiFiEspClient client = server.available();
    if (client)
    {
        boolean current = true;
        while (client.connected())
        {
            // Serial.println("Client Connected...");
            if (client.available())
            {
                // Serial.println("Read From Client...");
                char c = client.read();
                Serial.write(c);
                read_buffer += c;
                if (c == '\n')
                {
                    current = true;
                }
                else if (c != '\r')
                {
                    current = false;
                }
                if (c == '\n' && current)
                {
                    refresh_cnt++;
                    write_header(client);
                    write_html_body(client);
                    break;
                }
            }
        }
        delay(10);
        client.stop();
        Serial.println("Client disconnected...");
        for (int i = 0; i < read_buffer.length(); i++)
        {
            Serial.print(read_buffer[i],HEX);
        }
        read_buffer = "";
    }
}

/*
WiFi.status()
  WL_CONNECTED: assigned when connected to a WiFi network;
  WL_NO_SHIELD: assigned when no WiFi shield is present;
  WL_IDLE_STATUS: it is a temporary status assigned when WiFi.begin() is called and remains active until the number of attempts expires (resulting in WL_CONNECT_FAILED) or a connection is established (resulting in WL_CONNECTED);
  WL_NO_SSID_AVAIL: assigned when no SSID are available;
  WL_SCAN_COMPLETED: assigned when the scan networks is completed;
  WL_CONNECT_FAILED: assigned when the connection fails for all the attempts;
  WL_CONNECTION_LOST: assigned when the connection is lost;
  WL_DISCONNECTED: assigned when disconnected from a network;
*/
void wifi_Status_Check()
{
    if (status == CONNECTED_SSID)
    {
        Serial.print("Connected to SSID !! : ");
        Serial.println(ssid);
        ipaddress = WiFi.localIP();
        delay(10000);
        print_address("WiFi Connected....IPaddress is ", ipaddress);
        int iphead = ipaddress[0];
        Serial.println(String("IPHeader : ") + iphead);
        if (iphead != 0 && iphead != 255)
        {
            Serial.println("GOT IPADDRESS !!");
            status = GOT_IPADDRESS;
        }
        else
        {
            // status = WL_IDLE_STATUS;
        }

        if (status == GOT_IPADDRESS)
        {
            server.begin();
            Serial.println("Server Stated....");
            status = SERVER_STARTED;
        }
    }
}

void write_header(WiFiEspClient client)
{
    // Serial.println("Send header to Client...");
    client.flush();
    client.print("HTTP/1.1 200 OK\r\n");
    client.print("Content-Type: text/html\r\n");
    client.print("Connection:close\r\n");
    client.print("Refresh: 20\r\n");
    client.print("\r\n");
}

void write_html_body(WiFiEspClient client)
{
    // Serial.print("Send body to Client...");
    client.print("<!DOCTYPE html>\r\n");
    client.print("<html lang='kor'>\r\n");
    client.print("<head>\r\n");
    client.print("<meta charset='UTF-8'>\r\n");
    client.print("<title>Arduino Ethernet Server</title>\r\n");
    client.print("</head>\r\n");
    client.print("<body>\r\n");
    client.print("<h1>Wellcome SJ WiFi Server!!</h1>\r\n");
    client.print("connected Counter : ");
    client.print(refresh_cnt);
    client.print("\r\n");
    client.print("</body>\r\n");
    client.print("</html>\r\n");
    client.print("\r\n");
}

void print_address(String header, IPAddress address)
{
    Serial.print(header);
    Serial.print("[");
    for (int i = 0; i < 4; i++)
    {
        Serial.print(address[i], DEC);
        if (i != 3)
        {
            Serial.print(".");
        }
    }
    Serial.println("]");
    // int iphead = address[0];
    // Serial.println(String("IPHeader : ") + iphead);
}