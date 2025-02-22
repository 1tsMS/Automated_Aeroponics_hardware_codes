
#include <DHT.h>
#define DHT11_PIN  21 // ESP32 pin GPIO21 connected to DHT11 sensor

DHT dht(DHT11_PIN, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin(); // initialize the DHT11 sensor
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  

    

    Serial.print("Temperature : ");
    Serial.println(t);

    Serial.print("    Humidity : ");
    Serial.println(h);

  // wait a 2 seconds between readings
  delay(2000);
}