#include <SoftwareSerial.h>
#include <DHT.h>

#define DHT_PIN A0

SoftwareSerial BTSerial(3, 2);
DHT dht(A0, DHT11);

float mTemperature;
float mHumidity;
int btn_pin[] = {4, 5, 6};
int led_pin[] = {8, 9, 10};
boolean btn_Last_Status[] = {LOW, LOW, LOW};
boolean btn_Current_Status[] = {LOW, LOW, LOW};
boolean led_On[] = {LOW, LOW, LOW};
String led_state[] = {"", "", ""};

void setup()
{
    Serial.begin(9600);
    BTSerial.begin(9600);
    dht.begin();
    for (int i = 0; i < 3; i++)
    {
        pinMode(btn_pin[i], INPUT);
        pinMode(led_pin[i], OUTPUT);
    }
}

void loop()
{
    mTemperature = dht.readTemperature();
    mHumidity = dht.readHumidity();

    for (int i = 0; i < 3; i++)
    {
        btn_Current_Status[i] = debounce(btn_Last_Status[i], btn_pin[i]);
        if (btn_Last_Status[i] == LOW && btn_Current_Status[i] == HIGH)
        {
            led_On[i] = !led_On[i];
            char message[5] = "L  \n";
            message[1] = i + 1 + '0';
            message[2] = led_On[i] + '0';
            led_state[i] = message;
            delay(200);
        }
        btn_Last_Status[i] = btn_Current_Status[i];
        digitalWrite(led_pin[i], led_On[i]);
    }
    for(int i = 0 ; i<3 ; i++){
        if(!led_state[i].equals("")){
            BTSerial.print(led_state[i]);
        }
    }
    BTSerial.print(String("T") + int(mTemperature) + String("\n"));
    BTSerial.print(String("H") + int(mHumidity) + String("\n"));
}

boolean debounce(boolean last, int btn_pin)
{
    boolean current = digitalRead(btn_pin);
    if (last != current)
    {
        delay(5);
        current = digitalRead(btn_pin);
    }
    return current;
}
