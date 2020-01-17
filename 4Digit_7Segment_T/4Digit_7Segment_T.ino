// number patterns for 0 to 9
byte patterns[] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xE6};
//byte patterns[] = {0x03, 0x9F, 0x25, 0x0D, 0x99, 0x49, 0x41, 0x1F, 0x01, 0x09};

// number of digits
int digit_seletct_pin[] = {2, 3, 4, 5};
int segment_pin[] = {6, 7, 8, 9, 10, 11, 12, 13};
//int segment_pin[] = {13, 12, 11, 10, 9, 8, 7, 6};
unsigned long time_previous, time_current;
int minutes = 0, seconds = 0;

void setup() {
  for (int i = 0 ; i < 4; i++) {
    pinMode(digit_seletct_pin[i], OUTPUT);
  }

  for (int i = 0 ; i < 8; i++) {
    pinMode(segment_pin[i], OUTPUT);
  }
  time_previous = millis();
  Serial.begin(9600);
}

void loop() {
//  time_current = millis();
//  if (time_current - time_previous >= 1000) {
//    time_previous = time_current;
//    seconds++;
//    if (seconds == 60) {
//      seconds = 0;
//      minutes++;
//    }
//    if (minutes == 60 ) minutes = 0;
//
//  }
//  show_4_digit(minutes * 100 + seconds);
//  Serial.println(minutes * 100 + seconds);
   
    int reading = analogRead(A0);
    show_4_digit(reading);
    Serial.println(reading);

}

// Display Number on Each digit
void show_digit(int pos, int number) {
  for (int i = 0 ; i < 4 ; i++) {
    if (i + 1 == pos) {
      digitalWrite(digit_seletct_pin[i], HIGH);
      //      Serial.print("1");
    } else {
      digitalWrite(digit_seletct_pin[i], LOW);
      //      Serial.print("0");
    }
  }
  //  Serial.println("");

  for (int i = 0 ; i < 8 ; i++) {
    boolean on_off = bitRead(patterns[number], 7 - i);
    digitalWrite(segment_pin[i], !on_off);
    //    Serial.print(!on_off);
  }
  //  Serial.println("");
}

// Display 4Digits number
void show_4_digit(int number4) {
  int number = number4;
  number = number % 10000; //Limits 4digits
  int thousands = number / 1000;
  number = number % 1000;
  int hundreds = number / 100;
  number = number % 100;
  int tens = number / 10;
  int ones = number % 10;
  if (number4 > 999) show_digit(1, thousands);
  delay(5);
  if (number4 > 99) show_digit(2, hundreds);
  delay(5);
  if (number4 > 9) show_digit(3, tens);
  delay(5);
  show_digit(4, ones);
  delay(5);

}
