#include <SoftwareSerial.h>

SoftwareSerial WiFiSerial(2,3);

void setup() {
  Serial.begin(115200);
  WiFiSerial.begin(115200);
}

void loop() {
  if(Serial.available()){
    WiFiSerial.write(Serial.read());
  }

  if(WiFiSerial.available()){
    Serial.write(WiFiSerial.read());
  }

}