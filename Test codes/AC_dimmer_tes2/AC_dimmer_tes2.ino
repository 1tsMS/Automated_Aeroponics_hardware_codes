//Libraries
#include <RBDdimmer.h>

//Parameters
const int zeroCrossPin  = 32;
const int acdPin  = 33;
int MIN_POWER  = 20;
int MAX_POWER  = 90;
int POWER_STEP  = 5;

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
  for(power=MIN_POWER;power<=MAX_POWER;power+=POWER_STEP){
    acd.setPower(power); // setPower(0-100%);
      Serial.print("lampValue -> ");
      Serial.print(acd.getPower());
      Serial.println("%");
    delay(1000);
  }

  for(power=MAX_POWER;power>=MIN_POWER;power-=POWER_STEP){
    acd.setPower(power); // setPower(0-100%);
      Serial.print("lampValue -> ");
      Serial.print(acd.getPower());
      Serial.println("%");
    delay(1000);
  }
}