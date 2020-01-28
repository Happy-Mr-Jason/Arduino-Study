#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3);

int led_Pin[] = {4, 5, 6, 7, 8};
boolean led_State[5];

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
        //Amount of leds have to less than 10 (1~9)
        if (data >= '1' && data <= '5')
        {
            int index = data - '0' - 1;
            led_State[index] = !led_State[index];
            digitalWrite(led_Pin[index], led_State[index]);
            if(led_State[index]){
                Serial.println((index + 1) + String("번 전등 켜짐"));
            } else {
                Serial.println((index + 1) + String("번 전등 꺼짐"));
            }
        }
    }
}

