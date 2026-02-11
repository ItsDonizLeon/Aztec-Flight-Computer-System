#include <Arduino.h>
#include <Wire.h>
#include <MS5611.h>

// Create MS5611 object
MS5611 ms5611;

// Baseline (launch pad) pressure in Pascals
// This will be calibrated at startup
float baselinePressure = 101325.0f;

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Initialize MS5611 sensor
    Serial.println("Initializing MS5611...");
    if (!ms5611.begin()) {
        Serial.println("MS5611 NOT found!");
        while (1);  // Halt if sensor not detected
    }
    Serial.println("MS5611 detected!");

    // Calibrate baseline pressure at startup (launch pad reference)
    // Average a few readings to reduce noise
    const int samples = 20;
    float sum = 0.0f;

    for (int i = 0; i < samples; i++) {
        float p = ms5611.readPressure();
        if (p > 0.0f) {
            sum += p;
        }
        delay(10);  // small delay during calibration only
    }

    baselinePressure = sum / samples;

    Serial.print("Baseline Pressure (Pa): ");
    Serial.println(baselinePressure, 0);
}

void loop() {

    // Read temperature (°C)
    float temperature = ms5611.readTemperature();

    // Read pressure (Pa)
    float pressure = ms5611.readPressure();

    // Basic sanity check for pressure read
    if (pressure <= 0.0f) {
        Serial.println("Pressure read error!");
        return;
    }

    // Calculate altitude relative to launch pad
    // Uses baseline pressure instead of fixed sea-level value
    float altitude = ms5611.getAltitude(pressure, baselinePressure);

    // Clean, formatted serial output
    Serial.print("Temp: ");
    Serial.print(temperature, 2);
    Serial.print(" °C   Pressure: ");
    Serial.print(pressure, 0);
    Serial.print(" Pa   Altitude: ");
    Serial.print(altitude, 2);
    Serial.println(" m");

    delay(200);  // unchanged as requested
}

