#include <SoftwareSerial.h>

#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523

SoftwareSerial BTSerial(3, 2);
const int speaker = 9;

int notes[8] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};
String status[8] = {"Do1", "Re", "Mi", "Fa", "Sol", "Ra", "Si", "Do2"};
String buffer = "";

void setup()
{
    Serial.begin(9600);
    BTSerial.begin(9600);
    pinMode(speaker, OUTPUT);
}

void loop()
{
    if (BTSerial.available())
    {
        byte data = BTSerial.read();
        // Serial.println(data);
        if (data == '\n')
        {
            Serial.println(buffer);
            if (buffer[0] == 'P')
            {
                int value = buffer.substring(1).toInt() - 1;
                if (value >= 0 && value <= 8)
                {
                    run_tone(notes[value]);
                    Serial.println(status[value]);
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

void run_tone(int note)
{
    tone(speaker, note, 100);
}