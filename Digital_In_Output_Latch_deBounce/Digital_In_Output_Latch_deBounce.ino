#define LED 9
#define BTN 2
boolean ledOn = false;  // Current LED Status
boolean lastBtnStatus = LOW; // Previous Button Status
boolean currentBtnStatus = LOW; // Current Button Staus

void setup() {
  pinMode(LED,OUTPUT);
  pinMode(BTN,INPUT);
  Serial.begin(9800);
}

void loop() {
  currentBtnStatus = debounce(lastBtnStatus);
  if(lastBtnStatus == LOW && currentBtnStatus == HIGH){ 
    ledOn = !ledOn;
    if(ledOn){
      Serial.println("켜짐");
    } else {
      Serial.println("꺼짐");
    }
  }
    lastBtnStatus = currentBtnStatus;
    digitalWrite(LED, ledOn);
}

boolean debounce(boolean last){
  boolean current = digitalRead(BTN); // Current Button Status
  if(last != current){
    delay(5);
    current = digitalRead(BTN);
  }
  return current;
}
