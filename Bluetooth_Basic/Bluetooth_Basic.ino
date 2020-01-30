#include <SoftwareSerial.h>
SoftwareSerial BTserial(3, 2);

char c = ' ';
void setup()
{
    Serial.begin(9600);
    Serial.println("Arduino is ready");
    Serial.println("Remember to select Both NL & CR in the serial monitor");
    // HC-06 default serial speed for AT mode is 9600
    BTserial.begin(9600);
}

void loop()
{
    // Keep reading from HC-06 and send to Arduino Serial Monitor
    if (BTserial.available())
    {
        c = BTserial.read();
        Serial.write(c);
    }
    // Keep reading from Arduino Serial Monitor and send to HC-06
    if (Serial.available())
    {
        c = Serial.read();
        BTserial.write(c);
    }
}
