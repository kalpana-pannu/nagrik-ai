// Add this near the top of combined_sensebox.ino, after your existing #defines:
unsigned long lastLogTime = 0;
const unsigned long LOG_INTERVAL = 5000; // log every 5 seconds

// Add this inside loop(), after your existing sensor reads (temp, humidity,
// lightState, vibrationState, gasValue, rainLevel are already being read above):

if (millis() - lastLogTime >= LOG_INTERVAL) {
  lastLogTime = millis();

  // CSV_DATA prefix makes it easy to filter these lines out later on the PC side
  Serial.print("CSV_DATA,");
  Serial.print(millis());       // timestamp (ms since boot)
  Serial.print(",");
  Serial.print(temp);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.print(lightState);
  Serial.print(",");
  Serial.print(vibrationState);
  Serial.print(",");
  Serial.print(gasValue);
  Serial.print(",");
  Serial.println(rainLevel);
}
