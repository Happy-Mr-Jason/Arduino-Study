#define DELAY_TIME 100
int led_pin[] = {2, 3, 4, 5, 6};
int state = 0;
void setup() {
  for(int i = 0 ; i < 5 ; i++){
     pinMode(led_pin[i], OUTPUT);
  }
  Serial.begin(9700);
}

void loop() {
  for(int i = 0 ; i < 5 ; i++){
    digitalWrite(led_pin[i], HIGH);  
    Serial.println(i + String("번핀    켜짐"));
    delay(DELAY_TIME);
    digitalWrite(led_pin[i], LOW);
    Serial.println(i + String("번핀 꺼짐"));
    delay(DELAY_TIME);
  }
  
}
