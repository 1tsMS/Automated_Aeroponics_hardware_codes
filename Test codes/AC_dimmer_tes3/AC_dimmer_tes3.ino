//Libraries
#include <RBDdimmer.h>

//Parameters
const int zeroCrossPin  = 32;
const int acdPin  = 33;
int MIN_POWER  = 20;
int MAX_POWER  = 90;
int POWER_STEP  = 5;
#define ldrpin 25


//Variables
int power  = 0;

//Objects
dimmerLamp acd(acdPin,zeroCrossPin);

void setup(){
//Init Serial USB
Serial.begin(115200);
Serial.println(F("ESP32 System"));
acd.begin(NORMAL_MODE, ON);
}

void loop(){
  testDimmer();
}

void testDimmer(){/* function testDimmer */ 
////Sweep light power to test dimmer
  int intensity = analogRead(ldrpin);
  float intensity_percent = (intensity/4095.0)*100;

  Serial.println(intensity);
  Serial.print("Percent Brightness: ");
  Serial.println(intensity_percent);

  if(intensity_percent<=20){
    acd.setPower(90);
  }
  else if(intensity_percent>20 && intensity_percent<=50){
    acd.setPower(70);
  }
  else if(intensity_percent>50 && intensity_percent<=80){
    acd.setPower(40);
  }
  else if(intensity_percent>80 && intensity_percent<=100){
    acd.setPower(0);
  }
  delay(1000);

}