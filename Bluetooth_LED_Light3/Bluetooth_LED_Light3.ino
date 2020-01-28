#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3);

int led_Pin[] = {4, 5, 6, 7, 8};
boolean led_State[5];
String buffer = "";

void setup()
{
    Serial.begin(9600);
    BTserial.begin(9600);
    for (int i = 0; i < sizeof(led_Pin); i++)
    {
        pinMode(led_Pin[i], OUTPUT);
        led_State[i] = false;
        digitalWrite(led_Pin[i], led_State[i]);
    }
}

void loop()
{
    if (BTserial.available())
    {
        byte data = BTserial.read();
        if (data == '\n')
        {
            int index = buffer.toInt();

            if (index >= 1 && index <= 5)
            {
                int arrayIndex = index - 1;
                led_State[arrayIndex] = !led_State[arrayIndex];
                digitalWrite(led_Pin[arrayIndex], led_State[arrayIndex]);
                if (led_State[arrayIndex])
                {
                    Serial.println(String("Number") + index + String(" LED ON"));
                }
                else
                {
                    Serial.println(String("Number") + index + String(" LED OFF"));
                }
            }
            buffer = "";
        }
        else
        {
            buffer += (char)data;
        }
    }
}
