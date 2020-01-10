const int POT = A0; // Analog Input Pin Number
int LED[] = {2,3,4,5};
int val = 0; // Volume Value
int count;
void setup() {
  Serial.begin(9600);
  pinMode(POT, INPUT);
  for(int i = 0; i < 4; i++){
  pinMode(LED[i], OUTPUT);  
  }
}

void loop() {
  val = analogRead(POT);
  count = (val >> 8) + 1;
  Serial.println(val);
    
  for(int i = 0; i < 4; i++){
    if(i < count){
      if(val == 0){
        digitalWrite(LED[i], LOW);
        count = 0;
      }else{
        digitalWrite(LED[i],HIGH);  
      }
    } else {
      digitalWrite(LED[i], LOW);
    }
  }
  Serial.println(String("저항값") + val + String("LED개수:") + count);
  delay(100);
}
