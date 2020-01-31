#include <SoftwareSerial.h>

int RED = 10;
int GREEN = 9;
int BLUE = 8;

SoftwareSerial BTSerial(3, 2);

int colorRed = 0;
int colorGreen = 0;
int colorBlue = 0;

String buffer = "";

void setup()
{
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    Serial.begin(9600);
    BTSerial.begin(9600);
}

void loop()
{
    if (BTSerial.available())
    {
        byte data = BTSerial.read();
        if (data == '\n')
        {
            Serial.println(buffer);   //Received Data Format : [RR255\n : RGB/RED/VALUE] [RG255\n : RGB/GREEN/VALUE] [RB255\n : RGB/BLUE/VALUE]
            if (buffer.length() > 1)
            {
                if (buffer[0] == 'R')
                {
                    int color = buffer.substring(2).toInt();
                    switch (buffer[1])
                    {
                        case 'R' :
                            analogWrite(RED, color);
                            break;
                        case 'G' :
                            analogWrite(GREEN, color);
                            break;
                        case 'B' :
                            analogWrite(BLUE, color);
                            break;
                    }
                }
            }
            buffer = "";
        } else {
            buffer += (char)data;
        }
    }

}
