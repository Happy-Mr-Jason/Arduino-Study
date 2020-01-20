#include <boarddefs.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremote.h>
#include <IRremoteInt.h>

#define RED 8
#define GREEN 9
#define BLUE 10

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // 적외선 값을 받아온다
  irrecv.blink13(true);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
}

void loop()
{

  if (irrecv.decode(&results)) {
    // Serial.println(results.value, HEX); //IR값을 읽어 온다.
    // irrecv.resume();     // IR값을 읽은 후 resume한다.
    switch (results.value) {
      case 0xFFA25D:
        Serial.println("CH-");
        break;
      case 0xFF629D:
        Serial.println("CH");
        break;
      case 0xFFE21D:
        Serial.println("CH+");
        break;
      case 0xFF22DD:
        Serial.println("PREV");
        break;
      case 0xFF02FD:
        Serial.println("NEXT");
        break;
      case 0xFFC23D:
        Serial.println("PLAY/PAUSE");
        break;
      case 0xFFE01F:
        Serial.println("VOL-");
        break;
      case 0xFFA857:
        Serial.println("VOL+");
        break;
      case 0xFF906F:
        Serial.println("EQ");
        break;
      case 0xFF6897:
        Serial.println("0");
        led_light(0);
        break;
      case 0xFF9867:
        Serial.println("100+");
        break;
      case 0xFFB04F:
        Serial.println("200+");
        break;
      case 0xFF30CF:
        Serial.println("1");
        led_light(1);
        digitalWrite(RED, HIGH);
        break;
      case 0xFF18E7:
        Serial.println("2");
        led_light(2);
        digitalWrite(GREEN, HIGH);
        break;
      case 0xFF7A85:
        Serial.println("3");
        digitalWrite(BLUE, HIGH);
        led_light(3);;
        break;
      case 0xFF10EF:
        Serial.println("4");
        led_light(4);
        break;
      case 0xFF38C7:
        Serial.println("5");
        led_light(5);
        break;
      case 0xFF5AA5:
        Serial.println("6");
        break;
      case 0xFF42BD:
        Serial.println("7");
        break;
      case 0xFF4AB5:
        Serial.println("8");
        break;
      case 0xFF52AD:
        Serial.println("9");
        break;
    }
    irrecv.resume();
  }
}

void led_light(int button) {
  if (button == 0) {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
  } else if (button == 1) {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
  } else if (button == 2) {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);
  } else if (button == 3) {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);
  } else if (button == 4) {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, HIGH);
  } else if (button == 5) {
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW;
  }else {
    
  }
}
