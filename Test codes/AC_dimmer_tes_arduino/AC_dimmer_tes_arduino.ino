//Libraries
#include <RBDdimmer.h>

//Parameters
const int acdPin = 9; // Dimmer control pin
int MIN_POWER = 20;
int MAX_POWER = 90;
int POWER_STEP = 5;
#define ldrpin 25

//Variables
int power = 0;

//Objects
dimmerLamp acd(acdPin); // FIXED: Only passing the dimmer pin

void setup() {
  Serial.begin(115200);
  Serial.println(F("ESP32 System"));
  pinMode(ldrpin,INPUT);
  acd.begin(NORMAL_MODE, ON);
}

void loop() {
  testDimmer();
}

void testDimmer() {
  int intensity = analogRead(ldrpin);
  float intensity_percent = (intensity / 4095.0) * 100;

  Serial.println(intensity);
  Serial.print("Percent Brightness: ");
  Serial.println(intensity_percent);

  if (intensity_percent <= 20) {
    acd.setPower(90);
  } else if (intensity_percent > 20 && intensity_percent <= 50) {
    acd.setPower(70);
  } else if (intensity_percent > 50 && intensity_percent <= 80) {
    acd.setPower(40);
  } else if (intensity_percent > 80 && intensity_percent <= 100) {
    acd.setPower(0);
  }
  
  delay(1000);
}
