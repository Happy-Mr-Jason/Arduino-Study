#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 7
#define DHTTYPE DHT11
#define LED_1 8   //DRY
#define LED_2 9
#define LED_3 10
#define LED_4 11  //WET
// Create DHT Object
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(LED_1, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_4, OUTPUT);
  Serial.begin(9600);
  //DHT Sensing start
  dht.begin();
}

void loop() {
  delay(2000);
  float h = dht.readHumidity(); // Measure Humidity
  float t = dht.readTemperature(); // Measure Temperature 'C
  float f = dht.readTemperature(true); // Measure Temperature 'F
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(String("온도 센서 측정 오류"));
  }
  float hif = dht.computeHeatIndex(f, h); // Compensated 'F
  float hic = dht.computeHeatIndex(t, h, false); // Compensated 'C
  Serial.print(String("습도 : ") + h);
  Serial.print(String(" 온도 : ") + t);
  Serial.print(String(" 온도(화씨) : ") + f);
  Serial.print(String(" 보정된온도 : ") + hic);
  Serial.println(String(" 보정된온도(화씨) : ") + hif);

  if (h > 30 && h <= 50 ) {
    digitalWrite(LED_1, HIGH);
  } else {
    digitalWrite(LED_1, LOW);
  }
  if (h > 50 && h <= 70) {
    digitalWrite(LED_2, HIGH);
  } else {
    digitalWrite(LED_2, LOW);
  }
  if (h > 70 && h <= 90) {
    digitalWrite(LED_3, HIGH);
  } else {
    digitalWrite(LED_3, LOW);
  }
  if (h > 90) {
    digitalWrite(LED_4, HIGH);
  } else {
    digitalWrite(LED_4, LOW);
  }
}
