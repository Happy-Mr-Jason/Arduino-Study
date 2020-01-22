#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3);

char c = ' ';
void setup()
{
    Serial.begin(9600);
    Serial.println("Arduino is ready");
    Serial.println("Remember to select Both NL & CR in the serial monitor");
    // HC-05 default serial speed for AT mode is 38400
    BTserial.begin(9600);
}

void loop()
{
    // Keep reading from HC-05 and send to Arduino Serial Monitor
    if (BTserial.available())
    {
        c = BTserial.read();
        Serial.write(c);
    }
    // Keep reading from Arduino Serial Monitor and send to HC-05
    if (Serial.available())
    {
        c = Serial.read();
        BTserial.write(c);
    }
}
