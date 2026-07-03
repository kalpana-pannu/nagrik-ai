#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("NAGRIK AI - DHT22 Ready");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Sensor Error!");
    delay(2000);
    return;
  }

  if (t > 45) {
    Serial.println("HEAT ALERT");
  }
  else if (t >= 38 && t < 45) {
    Serial.println("WARNING - High Temperature");
  }
  else {
    Serial.print("Temp is normal: ");
    Serial.print(t);
    Serial.print(" C  Humidity: ");
    Serial.print(h);
    Serial.println(" %");
  }

  delay(2000);
}
