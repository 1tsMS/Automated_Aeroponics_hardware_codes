
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


#define DHTPIN 21 //Connect Out pin to D2 in NODE MCU
#define PHPIN_A 31
#define MOISPIN_A 35
#define relaypin 2

#define TRIG_PIN 5  // Trigger pin
#define ECHO_PIN 18  // Echo pin

#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

//DHT 
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

//TDS Variables
namespace pin {
  const byte tds_sensor = 34;
}

namespace device {
  float aref = 3.3; // Vref, this is for 3.3v compatible controller boards, for Arduino use 5.0v.
}

namespace sensor {
  float ec = 0;
  unsigned int tds = 0;
  float ecCalibration = 1;
}

void ReadSend_Tds() {
    // Read the raw analog value and convert to voltage
    float rawEc = analogRead(pin::tds_sensor) * device::aref / 1024.0;
    
    // Debugging: Print the raw analog value
    Serial.print(F("Raw Analog Value: "));
    Serial.println(rawEc);

    // Adjust this offset based on the sensor's dry reading (without immersion)
    float offset = 0.14; // Set this to the observed raw analog value in air

    // Apply calibration and offset compensation
    sensor::ec = (rawEc * sensor::ecCalibration) - offset;

    // If the EC is below zero after adjustment, set it to zero
    if (sensor::ec < 0) sensor::ec = 0;

    // Convert voltage value to TDS value using a cubic equation
    sensor::tds = (133.42 * pow(sensor::ec, 3) - 255.86 * sensor::ec * sensor::ec + 857.39 * sensor::ec) * 0.5;

    // Debugging: Print the TDS and EC values
    Serial.print(F("TDS: "));
    Serial.print(sensor::tds);
    Serial.println("ppm");
    Serial.print(F("EC: "));
    Serial.print(sensor::ec, 2);
    Serial.print("mS/cm");
    
    Serial.println("------------------------");  // Separator line
    
    // Send data to Blynk virtual pins
    Blynk.virtualWrite(V2, sensor::tds);
    Blynk.virtualWrite(V3, sensor::ec);
}


// pH Sensor Variables
float calibration_value = 20.24 - 0.7; // Adjust calibration
int buffer_arr[10], temp;
float ph_act;
void ReadSend_PH() {
    unsigned long int avgval = 0;
  
    // Read sensor data 10 times
    for (int i = 0; i < 10; i++) { 
      buffer_arr[i] = analogRead(PHPIN_A);
      delay(30);
    }
  
    // Sort values to remove noise
    for (int i = 0; i < 9; i++) {
      for (int j = i + 1; j < 10; j++) {
        if (buffer_arr[i] > buffer_arr[j]) {
          temp = buffer_arr[i];
          buffer_arr[i] = buffer_arr[j];
          buffer_arr[j] = temp;
        }
      }
    }
  
    // Take the middle 6 values for averaging
    for (int i = 2; i < 8; i++) 
      avgval += buffer_arr[i];
  
    // Convert ADC value to voltage
    float volt = (float)avgval * 3.3 / 4096.0 / 6;
  
    // Convert voltage to pH
    ph_act = -5.70 * volt + calibration_value;
  
    // Print pH value to Serial Monitor
    Serial.print("pH Value: ");
    Serial.println(ph_act);
  
    // Send data to Blynk app (Virtual Pin V2)
    Blynk.virtualWrite(V4, ph_act);
  }


// Moisture Sensor Variables
const int dryValue  = 2559;   //you need to replace this value with Value_1
const int wetValue = 1215;
void ReadSend_Mois() {
    int value = analogRead(MOISPIN_A);
  // put your main code here, to run repeatedly:
  int soilMoisturePercent  = map(value, dryValue, wetValue, 0, 100);
  Serial.print("Soil Moisture : ");
  Serial.print(soilMoisturePercent);
  Serial.println("%");
    Blynk.virtualWrite(V5, soilMoisturePercent);
}


//Ultrasonic
void ReadSend_WaterLevel() {
  float TOTAL_HEIGHT = 28;

  long totalDuration = 0;
  int samples = 5;

  for (int i = 0; i < samples; i++) {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    totalDuration += duration;
    delay(10);
  }

  long avgDuration = totalDuration / samples;
  float distance = avgDuration * 0.0352 / 2;  // Convert to cm

  // Calculate water level percentage
  float waterLevel = ((TOTAL_HEIGHT - distance) / TOTAL_HEIGHT) * 100.0;
  if (waterLevel < 0) waterLevel = 0;
  if (waterLevel > 100) waterLevel = 100;

  Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
  Serial.print("Water Level: "); Serial.print(waterLevel); Serial.println(" %");


  Blynk.virtualWrite(V7, waterLevel);
  delay(500); // Delay for stability
}

void setup()
{   
  
  Serial.begin(115200);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(relaypin,OUTPUT);
  digitalWrite(relaypin, LOW);

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  
  timer.setInterval(1000L, Send_DHT);
  timer.setInterval(1000L, ReadSend_Tds);
  timer.setInterval(1000L, ReadSend_PH);
  timer.setInterval(1000L, ReadSend_Mois);
  timer.setInterval(2000L, ReadSend_WaterLevel);
  }

void loop()
{
  
  Blynk.run();
  timer.run();
 }


BLYNK_WRITE(V6) // this command is listening when something is written to V1
  {
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  
  if (pinValue == 1){
    digitalWrite(relaypin, HIGH);

   // do something when button is pressed;
  } else if (pinValue == 0) {
    digitalWrite(relaypin, LOW);
    // do something when button is released;
  }
  Serial.print("V1 button value is: "); // printing value to serial monitor
  Serial.println(pinValue);
  }

 