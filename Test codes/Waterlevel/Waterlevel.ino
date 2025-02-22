#define TRIG_PIN 5  // Trigger pin
#define ECHO_PIN 4  // Echo pin

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop() {
  float TOTAL_HEIGHT = 38.5;

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


  delay(500); // Delay for stability
}
