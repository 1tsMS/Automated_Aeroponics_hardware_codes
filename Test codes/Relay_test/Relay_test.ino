#define relay1 14                  //  Define Pin of the Relay

void setup(){

  pinMode(relay1,OUTPUT);         // Setting the Pin to output signal
  pinMode(34,INPUT);
  Serial.begin(9600);
}

void loop(){
  digitalWrite(relay1, LOW);    // Writing value "LOW" to the pin
  delay(10000);                  // Delay in miliseconds 
  digitalWrite(relay1, HIGH);   // Writing value "HIGH" to the pin
  delay(10000);

}