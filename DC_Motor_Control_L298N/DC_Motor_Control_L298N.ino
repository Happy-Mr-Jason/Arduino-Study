int IN1 = 8;
int IN2 = 9;
int IN3 = 10;
int IN4 = 11;

void setup()
{
    Serial.begin(9600);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

void loop()
{
    // Rotate the Motor A clockwise
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    Serial.println("Rotate Motor A CW");
    delay(2000);
    // Motor A Stop
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    Serial.println("Stop Motor A");
    delay(500);
    // Rotate the Motor B clockwise
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Rotate Motor B CW");
    delay(2000);
    // Motor B Stop
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    Serial.println("Stop Motor B");
    delay(500);
    // Rotates the Motor A counter-clockwise
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    Serial.println("Rotate Motor A CCW");
    delay(2000);
    // Motor A Stop
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    Serial.println("Stop Motor A");
    delay(500);
    // Rotates the Motor B counter-clockwise
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Rotate Motor B CCW");
    delay(2000);
    // Motor B Stop
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    Serial.println("Stop Motor B");
    delay(500);
}