#include <Ethernet.h>
#include <SPI.h>

byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC};
IPAddress IP(192,168,2,77);

void setup()
{
    Serial.begin(9600);
    if (Ethernet.begin(MAC) == 0)
    {
        Serial.println("DHCP Fail.......");
        Ethernet.begin(MAC, IP);
    }
}

void loop()
{
    IPAddress address;
    Serial.print("IP Address : ");
    address = Ethernet.localIP();
    print_address(address);
}

void print_address(IPAddress address)
{
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