#include <SoftwareSerial.h>

SoftwareSerial BTSerial(3, 2);

int pin = A0;
// for static monitoring
int value_previous, value_current;

void setup()
{
    Serial.begin(9600);
    BTSerial.begin(9600);
    value_previous = analogRead(pin);
}

void loop()
{
    int value = analogRead(pin);
    value_current = value;
    if (value_previous != value_current)
    {
        Serial.println(String("가변저항 값 = ") + value);
        Serial.println(String("전압 = ") + (value * 5 / 1023.0) + String("V"));
        BTSerial.println(String("V") + value);
        value_previous = value_current;
    }
}

void sendData(int value)
{
    String message = String(value) + '\n';
    BTSerial.print(message);
}