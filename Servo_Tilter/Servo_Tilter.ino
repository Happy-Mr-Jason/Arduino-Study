#include <Servo.h>

Servo mBase, mTilter;
String buffer = "";
int baseAngle = 0;
int tilterAngle = 0;
int baseOffset = 3;
int tilterOffset = 0;

void setup()
{
    mBase.attach(10);
    mTilter.attach(11);
    Serial.begin(9600);
    baseAngle = 90;
    tilterAngle = 90;
    mBase.write(baseAngle);
    mTilter.write(tilterAngle);
}

void loop()
{
    if (Serial.available())
    {
        byte c = Serial.read();
        if (c == '\n')
        {
            if (buffer[0] == 'B')
            {
                baseAngle = buffer.substring(1).toInt();
                Serial.println(String("OK Base to ") + baseAngle);
            }
            else if (buffer[0] == 'T')
            {
                tilterAngle = buffer.substring(1).toInt();
                Serial.println(String("OK Tilter to ") + tilterAngle);
            }
            buffer = "";
        }
        else
        {
            buffer += char(c);
        }
    }

    mBase.write(baseAngle - baseOffset);
    mTilter.write(tilterAngle - tilterOffset);
}
