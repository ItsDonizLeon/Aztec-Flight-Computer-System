#include <Arduino.h>
#include <Wire.h>
#include <MS5611.h>

// Create MS5611 object
MS5611 ms5611;

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Initialize MS5611
    Serial.println("Initializing MS5611...");
    if (!ms5611.begin()) {
        Serial.println("MS5611 NOT found!");
        while (1);
    }
    Serial.println("MS5611 detected!");
}

void loop() {

    // Read temperature (returns °C)
    double temperature = ms5611.readTemperature();
    // Read pressure (returns Pa)
    double pressure = ms5611.readPressure();
    // Calculate altitude in meters
    double altitude = ms5611.getAltitude(pressure, 101325.0);  

    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" °C   Pressure: ");
    Serial.print(pressure);
    Serial.print(" Pa   Altitude: ");
    Serial.print(altitude);
    Serial.println(" m");

    delay(200);
}
