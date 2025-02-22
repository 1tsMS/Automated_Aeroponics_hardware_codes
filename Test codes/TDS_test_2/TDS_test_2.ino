#include <Arduino.h>
#include <OneWire.h>
#include <DFRobot_ESP_PH_WITH_ADC.h> 
#include <DFRobot_ESP_EC.h>
#include <DallasTemperature.h>
#include <Adafruit_ADS1015.h>     
#include <EEPROM.h>
#include <SimpleTimer.h>


#define ONE_WIRE_BUS 13                // this is the gpio pin 13 on esp32.
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
DFRobot_ESP_EC ec;
DFRobot_ESP_PH_WITH_ADC ph;
Adafruit_ADS1115 ads;
 
float phvoltage, phValue, phtemperature = 25;
float voltage, ecValue, temperature = 25;

float readTemperature()
{
  //add your code here to get the temperature from your temperature sensor
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
  
}

SimpleTimer timer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  EEPROM.begin(32);//needed EEPROM.begin to store calibration k in eeprom
  ph.begin();
  ec.begin();
  sensors.begin();
 
 
  timer.setInterval(1000L, []() { MainFunction(); });
}

void loop() {
  // put your main code here, to run repeatedly:
  timer.run();
}

void MainFunction()
{
 
 
    voltage = analogRead(A0); // A0 is the gpio 36 
    //Serial.print("voltage:");
    //Serial.println(voltage, 4);
 
    temperature = readTemperature();  // read your temperature sensor to execute temperature compensation
    Serial.print("temperature:");
    Serial.print(temperature, 1); 
    Serial.println("^C");
 
    ecValue = ec.readEC(voltage, temperature); // convert voltage to EC with temperature compensation
    Serial.print("EC:");
    //Serial.print(ecValue, 4);
    //Serial.println("ms/cm");
    Serial.println(ecValue); 
 
    
 
// Sensor Values to Blynk application

  ec.calibration(voltage, temperature); // calibration process by Serail CMD
 
}
