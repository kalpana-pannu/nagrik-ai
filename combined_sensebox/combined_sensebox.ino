// NAGRIK AI - Sense Box Combined Firmware
// Integrates DHT22, LDR, SW-420, MQ-135, Rain Sensor, and Buzzer
// KY-038 Noise Sensor omitted - current unit suspected defective (see ky038_noise.ino)

#include <DHT.h>

// ---- Pin Definitions (from pin-mapping.md) ----
#define DHTPIN 4          // adjust to your actual DHT22 pin
#define DHTTYPE DHT22

#define LDR_PIN 15
#define VIBRATION_PIN 27
#define MQ135_PIN 34
#define RAIN_PIN 33
#define BUZZER_PIN 25

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LDR_PIN, INPUT);
  pinMode(VIBRATION_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  dht.begin();

  Serial.println("========================================");
  Serial.println("NAGRIK AI - Sense Box Initializing...");
  Serial.println("MQ-135 warming up (20s)...");
  Serial.println("========================================");
  delay(20000); // MQ-135 warm-up period
  Serial.println("Sense Box Ready.");
}

void loop() {
  // ---- DHT22 ----
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("DHT22: Read failed");
  } else {
    Serial.print("Temp: ");
    Serial.print(temp);
    Serial.print(" C | Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");
  }

  // ---- LDR ----
  int lightState = digitalRead(LDR_PIN);
  if (lightState == LOW) {
    Serial.println("LDR: STREETLIGHT FAILURE ALERT - Dark detected");
  } else {
    Serial.println("LDR: Light Level Normal - Bright");
  }

  // ---- SW-420 Vibration ----
  int vibrationState = digitalRead(VIBRATION_PIN);
  if (vibrationState == HIGH) {
    Serial.println("SW-420: VIBRATION DETECTED - Possible road damage");
  } else {
    Serial.println("SW-420: Road Stable - No vibration");
  }

  // ---- MQ-135 Air Quality ----
  int gasValue = analogRead(MQ135_PIN);
  Serial.print("MQ-135: Air Quality Reading: ");
  Serial.println(gasValue);

  // ---- Rain Sensor ----
  int rainLevel = analogRead(RAIN_PIN);
  bool waterlogging = false;
  if (rainLevel < 2000) { // flip comparison if your module reads opposite
    Serial.print("Rain: WATERLOGGING/RAIN ALERT - Level: ");
    Serial.println(rainLevel);
    waterlogging = true;
  } else {
    Serial.print("Rain: Dry - Level: ");
    Serial.println(rainLevel);
  }

  // ---- Buzzer trigger logic ----
  // Sounds buzzer if any critical alert condition is active
  if (lightState == LOW || vibrationState == HIGH || waterlogging || gasValue > 2500) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  Serial.println("----------------------------------------");
  delay(1000);
}
