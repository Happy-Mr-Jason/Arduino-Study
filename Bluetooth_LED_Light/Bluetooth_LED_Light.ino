#include <SoftwareSerial.h>
SoftwareSerial BTserial(2, 3);

#define RED 6
#define GREEN 5
#define WHITE 4

char readChar = ' ';
String readaline = "";
String readCommand = "";
void setup()
{
    Serial.begin(9600);
    BTserial.begin(9600);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(WHITE, OUTPUT);
}

void loop()
{
    if (BTserial.available())
    {
        readChar = BTserial.read();
        Serial.write(readChar);
        readaline += readChar;
        if (readChar == '\n')
        {
            readCommand = readaline;
            readaline = "";
            Serial.println("Received Command : " + readCommand);
            process_BT_CMD();
        }
    }
}

void process_BT_CMD()
{
    if (readCommand.equals("LED RED ON\n"))
    {
        digitalWrite(RED, HIGH);
    }
    if (readCommand.equals("LED RED OFF\n"))
    {
        digitalWrite(RED, LOW);
    }
    if (readCommand.equals("LED GREEN ON\n"))
    {
        digitalWrite(GREEN, HIGH);
    }
    if (readCommand.equals("LED GREEN OFF\n"))
    {
        digitalWrite(GREEN, LOW);
    }
    if (readCommand.equals("LED WHITE ON\n"))
    {
        digitalWrite(WHITE, HIGH);
    }
    if (readCommand.equals("LED WHITE OFF\n"))
    {
        digitalWrite(WHITE, LOW);
    }
}

void return_CMD_to_BT(String return_msg){
    

}