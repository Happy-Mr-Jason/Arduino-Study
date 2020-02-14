#include <SD.h>
#include <SPI.h>
#include <Ethernet.h>

//MAC address for Arduino
byte MAC[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC};
//IP address for Arduino
IPAddress IP(192, 168, 2, 77);
//Create a Web-server
EthernetServer server(80);

//File object
File webFile;

void setup()
{
    Serial.begin(9600);
    Ethernet.begin(MAC, IP);
    delay(2000);
    server.begin();
    Serial.println("Finding SD cards....");
    if(!SD.begin(4)){
        Serial.println("Error - There is no SD card!!");
        return;
    }else{
        Serial.println("Find a SD card Successfully!");
    }

    if(!SD.exists("index.htm")){
        Serial.println("Error - Can't Find the index.htm file!!");
    }else{
        Serial.println("Find the index.htm Successfully!");
    }

     if(!SD.exists("image.png")){
        Serial.println("Error - Can't Find the image.png file!!");
    }else{
        Serial.println("Find the image.png Successfully!");
    }
}

void loop()
{
    EthernetClient client = server.available();
    if(client){
        boolean current = true;
        while (client.connected()){
            if(client.available()){
                char c = client.read();
                if(current){
                    write_header(client);
                    read_file_transfer(client);
                }
                if(c != '\r'){
                    current = false;
                }
                delay(1);
                client.stop();
            }
        }
    }
}

void write_header(EthernetClient client)
{
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection:close");
    client.println();
}

void read_file_transfer(EthernetClient client){
    webFile = SD.open("index.htm");
    if(webFile){
        while(webFile.available()){
            client.write(webFile.read());
        }
        webFile.close();
    }
}