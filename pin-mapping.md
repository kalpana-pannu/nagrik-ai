# NAGRIK SENSE BOX — GPIO Pin Mapping

Master pin assignment table for the ESP32-based Sense Box. Pins were chosen to avoid strapping pins and ADC2/WiFi conflicts where possible.

| Sensor | Signal Pin | Power | Status |
|---|---|---|---|
| DHT22 (Temp/Humidity) | — | 3.3V | ✅ Working |
| LDR (Light/Streetlight) | GPIO15 (DO) | 3.3V | ✅ Working — note: GPIO15 is a strapping pin, test boot behavior carefully |
| SW-420 (Vibration) | GPIO27 (DO) | 3.3V | ✅ Working |
| MQ-135 (Air Quality) | GPIO34 (AO) | 5V/VIN (required — 3.3V causes brownout) | ✅ Working |
| Rain Sensor | GPIO33 (AO) | 3.3V | ✅ Working |
| Buzzer (output) | GPIO25 | 3.3V | ✅ Working |
| KY-038 (Noise) | GPIO32 (AO, placeholder) | 3.3V | ⚠️ Pending — current unit suspected defective, not responding. Replace before final integration. |

## Wiring Notes
- Wire color convention: red = power, black = ground, yellow = digital signal, blue = analog signal
- Each sensor pin must sit on its own breadboard row — never share a row between VCC/GND/signal
- MQ-135 must be powered from 5V/VIN, not 3.3V, due to its heater's current draw
- GPIO15 is a boot strapping pin — if you see boot loops/upload issues after wiring LDR, this is the first suspect
