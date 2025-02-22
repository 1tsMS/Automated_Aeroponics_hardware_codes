#define BLYNK_TEMPLATE_ID "TMPL3T2tMsW39"
#define BLYNK_TEMPLATE_NAME "Aeroponics Sensors"
#define BLYNK_AUTH_TOKEN "gKFTG4U6X9SH9DTElFZB1t4fBX0YGA__"

#define BLYNK_PRINT Serial

#include <WiFi.h>
//#include <ESP8266WiFi.h> 
#include <BlynkSimpleEsp32.h>


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Mr Samosa";  // type your wifi name
char pass[] = "whoknows?";  // type your wifi password

BlynkTimer timer;

#define relaypin 2
void setup()
{   
  
  Serial.begin(115200);
  pinMode(relaypin,OUTPUT);
  digitalWrite(relaypin, LOW);

  Blynk.begin(auth, ssid, pass);
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

void loop()
{

  Blynk.run();
}