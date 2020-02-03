#define IR_DETECT 8
#define LED 7

void setup()
{
    pinMode(IR_DETECT, INPUT);
    pinMode(LED, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    if(digitalRead(IR_DETECT) == HIGH){
        Serial.println("IR Detected");
        digitalWrite(LED, HIGH);
    } else {
        digitalWrite(LED, LOW);
    }
    
}