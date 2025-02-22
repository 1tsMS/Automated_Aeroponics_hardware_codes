namespace pin {
    const byte tds_sensor = 34; // Adjust this pin as per ESP32 wiring
}

namespace device {
    const float aref = 3.3;  // ESP32 uses 3.3V as reference voltage
}

namespace sensor {
    float ec = 0;
    unsigned int tds = 0;
    const float ecCalibration = 1.0;  // Static calibration factor
    const float offset = 0.14;  // Static offset value
    const float temperature = 25.0; // Default temperature for compensation
}

// Function to apply temperature compensation
float applyTemperatureCompensation(float ec, float temp) {
    float tempCoefficient = 1.0 + 0.02 * (temp - 25.0); // Adjusts EC for temperature variations
    return ec / tempCoefficient;
}

void setup() {
    Serial.begin(115200);
    pinMode(pin::tds_sensor,INPUT);
    analogSetAttenuation(ADC_0db); // 0dB = 0-1.1V range, change if required
    analogReadResolution(12); // ESP32 supports 12-bit ADC (0-4095)
}

void loop() {

    // Read analog sensor value and convert to voltage
    float rawEc = analogRead(pin::tds_sensor) * device::aref / 4095.0; // ESP32 uses 12-bit ADC

    // Debugging: Print the raw analog value
    Serial.print(F("Raw Analog Value: "));
    Serial.println(rawEc);

    // Apply calibration and offset compensation
    sensor::ec = (rawEc * sensor::ecCalibration) - sensor::offset;

    // Ensure EC is not negative
    if (sensor::ec < 0) sensor::ec = 0;

    // Apply temperature compensation
    sensor::ec = applyTemperatureCompensation(sensor::ec, sensor::temperature);

    // Convert EC to TDS using the cubic equation
    sensor::tds = (133.42 * pow(sensor::ec, 3) - 255.86 * sensor::ec * sensor::ec + 857.39 * sensor::ec) * 0.5;

    // Debugging: Print the TDS and EC values
    Serial.print(F("TDS: "));
    Serial.println(sensor::tds);
    Serial.print(F("EC: "));
    Serial.println(sensor::ec, 2);
    Serial.print(F("Temperature: "));
    Serial.println(sensor::temperature);

    delay(1000);
}
