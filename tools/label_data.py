import serial
import csv
from datetime import datetime

PORT = "/dev/cu.usbserial-0001"
BAUD_RATE = 115200
OUTPUT_FILE = "nagrik_sensor_log.csv"

ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
print(f"Listening on {PORT}... logging to {OUTPUT_FILE}")

with open(OUTPUT_FILE, "a", newline="") as f:
    writer = csv.writer(f)

    if f.tell() == 0:
        writer.writerow(["logged_at", "millis", "temp", "humidity",
                          "light_state", "vibration_state", "gas_value", "rain_level"])

    while True:
        try:
            line = ser.readline().decode("utf-8").strip()
            if line.startswith("CSV_DATA,"):
                parts = line.split(",")[1:]
                row = [datetime.now().isoformat()] + parts
                writer.writerow(row)
                f.flush()
                print("Logged:", row)
        except KeyboardInterrupt:
            print("Stopped logging.")
            break
        except Exception as e:
            print("Error:", e)

