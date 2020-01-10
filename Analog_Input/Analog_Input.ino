const int POT = A0; // Analog Input Pin Number
int LED[] = {2,3,4,5};
int val = 0; // Volume Value

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < 4; i++){
  pinMode(LED[i], OUTPUT);  
  }
}

void loop() {
  val = analogRead(POT);
  Serial.println(val);
    
  String val_str = String(val);
  int str_length = val_str.length();

  if(val > 0){
    for(int i = 0 ; i < str_length ; i++){
      digitalWrite(LED[i], HIGH);
    }
  
    for(int j = str_length ; j < 4 ; j++){
      digitalWrite(LED[j], LOW); 
    }
  } else {
    for(int j = 0 ; j < 4 ; j++){
      digitalWrite(LED[j], LOW); 
    } 
  }

  Serial.println(String("저항값") + val + String("LED개수:") + str_length);
  delay(100);

}
