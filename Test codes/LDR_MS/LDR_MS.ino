#define ldrpin 13


void setup() {
  // put your setup code here, to run once:
  pinMode(ldrpin,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int intensity = analogRead(ldrpin);
  float intensity_percent = (intensity/4095.0)*100;

  Serial.println(intensity);
  Serial.print("Percent Brightness: ");
  Serial.println(intensity_percent);
  delay(500);
}
