#define RED 11
#define GREEN 10
#define BLUE 9
#define BTN 2

boolean ledOn = false;  // Current LED Status
int ledMode = 0 ;
boolean lastBtnStatus = LOW; // Previous Button Status
boolean currentBtnStatus = LOW; // Current Button Staus
int color[] = {11, 10, 9};

void setup() {
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  pinMode(BTN,INPUT);
}

void loop() {
   currentBtnStatus = debounce(lastBtnStatus);
  if(lastBtnStatus == LOW && currentBtnStatus == HIGH){ 
    ledOn = !ledOn;
    ledMode++;
    if(ledOn){
      Serial.println("켜짐");
    } else {
      Serial.println("꺼짐");
    }
  }
    lastBtnStatus = currentBtnStatus;
    if(ledMode == 8){
      ledMode = 0;
    }
    setMode(ledMode);
}

boolean debounce(boolean last){
  boolean current = digitalRead(BTN); // Current Button Status
  if(last != current){
    delay(5);
    current = digitalRead(BTN);
  }
  return current;
}

void setMode(int mode){
  
  if(mode == 1){
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);  
    digitalWrite(BLUE, LOW);  
  } else if(mode == 2){
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);  
    digitalWrite(BLUE, LOW);
  } else if(mode == 3){
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);  
    digitalWrite(BLUE, HIGH);
  } else if(mode == 4){     //보라색
    analogWrite(RED, 127);
    analogWrite(GREEN, 0);  
    analogWrite(BLUE, 127);
  } else if(mode == 5){     //청록색
    analogWrite(RED, 0);
    analogWrite(GREEN, 127);  
    analogWrite(BLUE, 127);
  } else if(mode == 6){     //orange
    analogWrite(RED, 127);
    analogWrite(GREEN, 127);  
    analogWrite(BLUE, 0);
  } else if(mode == 7){     //white
    analogWrite(RED, 85);
    analogWrite(GREEN, 85);  
    analogWrite(BLUE, 85);
  } else {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);  
    digitalWrite(BLUE, LOW);
  }
}
