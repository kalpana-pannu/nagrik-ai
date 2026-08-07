// ==========================================================
// NAGRIK AI - Sense Box Combined Firmware
// ESP32 + TinyML + DHT22 + MQ135 + LDR + Rain + SW420
// Part 1 : Includes, Global Variables and Setup()
// ==========================================================

#include "model_data.h"

#include <Chirale_TensorFlowLite.h>
#include <tensorflow/lite/micro/all_ops_resolver.h>
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/schema/schema_generated.h>

#include <DHT.h>

// ==========================================================
// Pin Definitions
// ==========================================================

#define DHTPIN 4
#define DHTTYPE DHT22

#define LDR_PIN 15
#define VIBRATION_PIN 27
#define MQ135_PIN 34
#define RAIN_PIN 33
#define BUZZER_PIN 25

// ==========================================================
// DHT Sensor
// ==========================================================

DHT dht(DHTPIN, DHTTYPE);

// ==========================================================
// TinyML Variables
// ==========================================================

const tflite::Model* tflModel = nullptr;
tflite::MicroInterpreter* tflInterpreter = nullptr;

TfLiteTensor* modelInput = nullptr;
TfLiteTensor* modelOutput = nullptr;

constexpr int kTensorArenaSize = 4 * 1024;
uint8_t tensorArena[kTensorArenaSize];

// ==========================================================
// StandardScaler values from Python training
// ==========================================================

float gasMean = 322.25588235294117;
float gasStd  = 228.75684970930888;

// ==========================================================
// CSV Logging
// ==========================================================

unsigned long lastLogTime = 0;
const unsigned long LOG_INTERVAL = 5000;

// ==========================================================
// SETUP
// ==========================================================

void setup()
{
    Serial.begin(115200);
    delay(1000);

    pinMode(LDR_PIN, INPUT);
    pinMode(VIBRATION_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    digitalWrite(BUZZER_PIN, LOW);

    dht.begin();

    Serial.println();
    Serial.println("========================================");
    Serial.println("NAGRIK AI Sense Box");
    Serial.println("ESP32 TinyML Starting...");
    Serial.println("========================================");

    Serial.println();
    Serial.println("Initializing Sensors...");
    Serial.println();

    Serial.println("MQ135 warming up...");
    Serial.println("Please wait 20 seconds...");
    delay(20000);

    // ======================================================
    // Load TinyML Model
    // ======================================================

    tflModel = tflite::GetModel(nagrik_gas_model_tflite);

    if (tflModel == nullptr)
    {
        Serial.println("ERROR: Model not found!");
        while (true);
    }

    if (tflModel->version() != TFLITE_SCHEMA_VERSION)
    {
        Serial.println("ERROR: Model schema mismatch!");
        while (true);
    }

    static tflite::AllOpsResolver resolver;

    static tflite::MicroInterpreter staticInterpreter(
        tflModel,
        resolver,
        tensorArena,
        kTensorArenaSize
    );

    tflInterpreter = &staticInterpreter;

    if (tflInterpreter->AllocateTensors() != kTfLiteOk)
    {
        Serial.println("ERROR: Tensor allocation failed!");
        while (true);
    }

    modelInput = tflInterpreter->input(0);
    modelOutput = tflInterpreter->output(0);

    Serial.println();
    Serial.println("TinyML Model Loaded Successfully");

    Serial.print("Tensor Arena : ");
    Serial.print(kTensorArenaSize);
    Serial.println(" bytes");

    Serial.print("Input Tensor : ");
    Serial.println(modelInput->bytes);

    Serial.print("Output Tensor: ");
    Serial.println(modelOutput->bytes);

    Serial.println();
    Serial.println("System Ready");
    Serial.println("========================================");
}
// ==========================================================
// LOOP
// ==========================================================

void loop()
{
    Serial.println();
    Serial.println("========================================");
    Serial.println("NAGRIK AI SENSOR STATUS");
    Serial.println("========================================");

    // ------------------------------------------------------
    // DHT22
    // ------------------------------------------------------

    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temp) || isnan(humidity))
    {
        Serial.println("DHT22 : Read Failed");
    }
    else
    {
        Serial.print("Temperature : ");
        Serial.print(temp);
        Serial.println(" °C");

        Serial.print("Humidity    : ");
        Serial.print(humidity);
        Serial.println(" %");
    }

    Serial.println();

    // ------------------------------------------------------
    // LDR
    // ------------------------------------------------------

    int lightState = digitalRead(LDR_PIN);

    if (lightState == LOW)
    {
        Serial.println("LDR : STREETLIGHT FAILURE (Dark)");
    }
    else
    {
        Serial.println("LDR : Light Normal");
    }

    Serial.println();

    // ------------------------------------------------------
    // SW420
    // ------------------------------------------------------

    int vibrationState = digitalRead(VIBRATION_PIN);

    if (vibrationState == HIGH)
    {
        Serial.println("SW420 : VIBRATION DETECTED");
    }
    else
    {
        Serial.println("SW420 : Road Stable");
    }

    Serial.println();

    // ------------------------------------------------------
    // MQ135
    // ------------------------------------------------------

    int gasValue = analogRead(MQ135_PIN);

    float scaledGas = (gasValue - gasMean) / gasStd;

    modelInput->data.f[0] = scaledGas;

    TfLiteStatus invokeStatus = tflInterpreter->Invoke();

    if (invokeStatus != kTfLiteOk)
    {
        Serial.println("TinyML Inference Failed!");
        delay(1000);
        return;
    }

    float anomalyScore = modelOutput->data.f[0];

    Serial.println("========== TinyML ==========");

    Serial.print("Gas Value     : ");
    Serial.println(gasValue);

    Serial.print("Scaled Value  : ");
    Serial.println(scaledGas, 4);

    Serial.print("Model Score   : ");
    Serial.println(anomalyScore, 4);

    Serial.print("Confidence    : ");
    Serial.print(anomalyScore * 100.0, 1);
    Serial.println("%");

    bool gasAlert = false;

    if (anomalyScore < 0.30)
    {
        Serial.println("Status : EXCELLENT AIR");
    }
    else if (anomalyScore < 0.50)
    {
        Serial.println("Status : NORMAL AIR");
    }
    else if (anomalyScore < 0.70)
    {
        Serial.println("Status : MODERATE POLLUTION");
        gasAlert = true;
    }
    else
    {
        Serial.println("Status : HIGH POLLUTION");
        gasAlert = true;
    }

    Serial.println("============================");
    Serial.println();

    // ------------------------------------------------------
    // Rain Sensor
    // ------------------------------------------------------

    int rainLevel = analogRead(RAIN_PIN);

    bool waterlogging = false;

    if (rainLevel < 2000)
    {
        Serial.print("Rain : WATER DETECTED  Level = ");
        Serial.println(rainLevel);

        waterlogging = true;
    }
    else
    {
        Serial.print("Rain : Dry  Level = ");
        Serial.println(rainLevel);
    }

    Serial.println();

    // ------------------------------------------------------
    // Buzzer
    // ------------------------------------------------------

    if (lightState == LOW ||
        vibrationState == HIGH ||
        waterlogging ||
        gasAlert)
    {
        digitalWrite(BUZZER_PIN, HIGH);

        Serial.println("BUZZER : ON");
    }
    else
    {
        digitalWrite(BUZZER_PIN, LOW);

        Serial.println("BUZZER : OFF");
    }

    Serial.println();

    // ------------------------------------------------------
    // CSV Logging
    // ------------------------------------------------------

    if (millis() - lastLogTime >= LOG_INTERVAL)
    {
        lastLogTime = millis();

        Serial.print("CSV_DATA,");

        Serial.print(millis());
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

        Serial.print(rainLevel);
        Serial.print(",");

        Serial.print(anomalyScore, 4);
        Serial.print(",");

        Serial.println(gasAlert);
    }

    Serial.println("----------------------------------------");

    delay(1000);
}
