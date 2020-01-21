#include <SPI.h>
#include <MFRC522.h>
#define RST_PIN 9
#define SS_PIN 10

#define LED_GREEN 3
#define LED_RED 4
#define BUZZER 6

MFRC522 mfrc522(SS_PIN, RST_PIN);
byte Access[] = {20, 4, 2, 43};
boolean ccard = true;

void setup()
{
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_RED, OUTPUT);
    pinMode(BUZZER, OUTPUT);
}

void loop()
{
    ccard = true;
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    for (int i = 0; i < 4; i++){
        Serial.print(mfrc522.uid.uidByte[i]);
        Serial.print(" ");
        if (mfrc522.uid.uidByte[i] != Access[i])
        {
            ccard = false;
        }
    }
    Serial.println();
    if(ccard == true) {
        digitalWrite(LED_GREEN, HIGH);
        digitalWrite(LED_RED, LOW);
        Serial.println("You are accepted.");
        tone(BUZZER, 523, 100);
        delay(500);
    }
    else {
        digitalWrite(LED_GREEN, LOW);
        digitalWrite(LED_RED, HIGH);
        Serial.println("Who are you?");
        tone(BUZZER, 523, 100);
        delay(300);
        tone(BUZZER, 523, 100);
        delay(500);
    }
}
