#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3);

byte buffer[1024];
int bufferPosition;

void setup()
{
    Serial.begin(9600);
    BTserial.begin(9600);
    bufferPosition = 0;
}

void loop()
{
    if(BTserial.available()){
        byte data = BTserial.read();
        Serial.write(data);
        buffer[bufferPosition++] = data;
        if(data == '\n'){
            buffer[bufferPosition] = '\0';
            //read from BT module
            Serial.print("보내는 문자 : ");
            Serial.write(buffer, bufferPosition);
            BTserial.write(buffer,bufferPosition);
            bufferPosition=0;
        }
    }
}
