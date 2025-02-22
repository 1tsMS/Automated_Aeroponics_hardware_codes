#define SensorPin 35 //Connect Out pin to D4 in ESP32
const int dryValue  = 2559;   //you need to replace this value with Value_1
const int wetValue = 1215; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  int value = analogRead(SensorPin);
  // put your main code here, to run repeatedly:
  int soilMoisturePercent  = map(value, dryValue, wetValue, 0, 100);
  Serial.print("Soil Moisture : ");
  Serial.println(soilMoisturePercent);
  delay(1000);
}
