//Tech Trends Shameer

#define BLYNK_TEMPLATE_ID "TMPL3T2tMsW39"
#define BLYNK_TEMPLATE_NAME "Aeroponics Sensors"
#define BLYNK_AUTH_TOKEN "gKFTG4U6X9SH9DTElFZB1t4fBX0YGA__"

#define BLYNK_PRINT Serial

#include <WiFi.h>
//#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp32.h>


#include <DHT.h>


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Mr Samosa";  // type your wifi name
char pass[] = "whoknows?";  // type your wifi password

BlynkTimer timer;


#define DHTPIN 37 //Connect Out pin to D2 in NODE MCU
#define TDSPIN 34

#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);


void Send_DHT()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  

    Blynk.virtualWrite(V0, t);
    Blynk.virtualWrite(V1, h);

    Serial.print("Temperature : ");
    Serial.print(t);

    Serial.print("    Humidity : ");
    Serial.println(h);
}


float aref = 3.3;  // Reference voltage
float ec = 0;      // Electrical conductivity
unsigned int tds = 0;  // Total dissolved solids
float ecCalibration = 1;

void ReadSend_Tds() {
    // Read the raw analog value and convert to voltage
    float rawEc = analogRead(TDSPIN) * aref / 1024.0;
    
    // Print raw reading
    Serial.print("Raw Analog Value: ");
    Serial.println(rawEc);

    // Adjust this offset based on the sensor's dry reading
    float offset = 0.14;
    
    // Apply calibration and offset compensation
    ec = (rawEc * ecCalibration) - offset;
    
    // If EC is below zero after adjustment, set it to zero
    if (ec < 0) ec = 0;
    
    // Convert voltage value to TDS value using cubic equation
    tds = (133.42 * pow(ec, 3) - 255.86 * ec * ec + 857.39 * ec) * 0.5;
    
    // Print values to Serial Monitor
    Serial.print("TDS Value: ");
    Serial.print(tds);
    Serial.println(" ppm");
    
    Serial.print("Electrical Conductivity Value: ");
    Serial.print(ec, 2);  // Print with 2 decimal places
    Serial.println(" mS/cm");
    
    Serial.println("------------------------");  // Separator line
    
    // Send data to Blynk virtual pins
    Blynk.virtualWrite(V2, tds);
    Blynk.virtualWrite(V3, ec);
}

void setup()
{   
  
  Serial.begin(115200);
  

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(500L, Send_DHT);
  timer.setInterval(500L, ReadSend_Tds);
 
  }

void loop()
{
  Blynk.run();
  timer.run();
 }