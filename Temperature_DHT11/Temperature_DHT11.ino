#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 7
#define DHTTYPE DHT11
// Create DHT Object
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  //DHT Sensing start
  dht.begin();
}

void loop() {
  delay(2000);
  float h = dht.readHumidity(); // Measure Humidity
  float t = dht.readTemperature(); // Measure Temperature 'C
  float f = dht.readTemperature(true); // Measure Temperature 'F
  if(isnan(h) || isnan(t) || isnan(f)){
    Serial.println(String("온도 센서 측정 오류"));
  }
  float hif = dht.computeHeatIndex(f, h); // Compensated 'F
  float hic = dht.computeHeatIndex(t,h,false); // Compensated 'C
  Serial.print(String("습도 : ") + h);
  Serial.print(String(" 온도 : ") + t);
  Serial.print(String(" 온도(화씨) : ") + f);
  Serial.print(String(" 보정된온도 : ") + hic);
  Serial.println(String(" 보정된온도(화씨) : ") + hif);
  
}
