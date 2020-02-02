#include <WiFi.h>
#include <SoftwareSerial.h>

SoftwareSerial WiFiSerial(3,2);

void setup()
{
  Serial.begin(115200);
  WiFiSerial.begin(115200);
}

void loop()
{
    if(Serial.available()){
        WiFiSerial.write(Serial.read());
    }

    if(WiFiSerial.available()){
        Serial.write(WiFiSerial.read());
    }
}
