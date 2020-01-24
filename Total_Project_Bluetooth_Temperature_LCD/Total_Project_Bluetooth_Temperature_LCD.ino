#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 7
#define DHTTYPE DHT11

SoftwareSerial BTserial(2, 3);
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

#define RED 6
#define GREEN 5
#define WHITE 4

char readChar = ' ';
String readaline = "";
String readCommand = "";
bool ledprocessing = false;
void setup()
{
    Serial.begin(9600);
    BTserial.begin(9600);
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(WHITE, OUTPUT);
    lcd.init();
    lcd.backlight();
    dht.begin();
}

void loop()
{
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    ledprocessing = BTserial.available();

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

    if (!ledprocessing)
    {   if(!BTserial.available())
        BTserial.print("Temperature," + String(temperature) + '\n');
        if(!BTserial.available())
        BTserial.print("Humidity," + String(humidity) + '\n');
    }

    lcd.setCursor(0, 0);
    lcd.print(temperature + String("C"));
    lcd.setCursor(0, 1);
    lcd.print(humidity + String("%"));
}

void process_BT_CMD()
{
    if (readCommand.equals("LED RED ON\n"))
    {
        digitalWrite(RED, HIGH);
        return_CMD_to_BT();
    }
    if (readCommand.equals("LED RED OFF\n"))
    {
        digitalWrite(RED, LOW);
        return_CMD_to_BT();
    }
    if (readCommand.equals("LED GREEN ON\n"))
    {
        digitalWrite(GREEN, HIGH);
        return_CMD_to_BT();
    }
    if (readCommand.equals("LED GREEN OFF\n"))
    {
        digitalWrite(GREEN, LOW);
        return_CMD_to_BT();
    }
    if (readCommand.equals("LED WHITE ON\n"))
    {
        digitalWrite(WHITE, HIGH);
        return_CMD_to_BT();
    }
    if (readCommand.equals("LED WHITE OFF\n"))
    {
        digitalWrite(WHITE, LOW);
        return_CMD_to_BT();
    }
}

void return_CMD_to_BT()
{
    BTserial.print(readCommand);
    readCommand = "";
    if (!BTserial.available())
    {
        ledprocessing = false;
    }
    
}