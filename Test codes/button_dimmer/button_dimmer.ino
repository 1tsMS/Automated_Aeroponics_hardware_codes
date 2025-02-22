// Blynk Credentials
#define BLYNK_TEMPLATE_ID "TMPL3T2tMsW39"
#define BLYNK_TEMPLATE_NAME "Aeroponics Sensors"
#define BLYNK_AUTH_TOKEN "gKFTG4U6X9SH9DTElFZB1t4fBX0YGA__"
#define BLYNK_PRINT Serial


// Libraries
#include <RBDdimmer.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>



char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Mr Samosa";  // WiFi name
char pass[] = "whoknows?";  // WiFi password

BlynkTimer timer;

// Dimmer Parameters
const int zeroCrossPin = 32;
const int acdPin = 33;
int MIN_POWER = 20;
int MAX_POWER = 90;
int POWER_STEP = 2;
int power = 0;

// Relay Pin
#define relaypin 2

// Objects
dimmerLamp acd(acdPin, zeroCrossPin);

void setup() {
    Serial.begin(115200);
    Serial.println(F("ESP32 System"));
    
    // Initialize AC Dimmer
    acd.begin(NORMAL_MODE, ON);
    
    // Initialize Relay
    pinMode(relaypin, OUTPUT);
    digitalWrite(relaypin, LOW);
    
    // Connect to Blynk
    Blynk.begin(auth, ssid, pass);
}

void loop() {
    Blynk.run();
    testDimmer();
}

void testDimmer() {
    // Sweep light power up
    for (power = MIN_POWER; power <= MAX_POWER; power += POWER_STEP) {
        acd.setPower(power);
        Serial.print("lampValue -> ");
        Serial.print(acd.getPower());
        Serial.println("%");
        delay(1000);
    }

    // Sweep light power down
    for (power = MAX_POWER; power >= MIN_POWER; power -= POWER_STEP) {
        acd.setPower(power);
        Serial.print("lampValue -> ");
        Serial.print(acd.getPower());
        Serial.println("%");
        delay(1000);
    }
}

BLYNK_WRITE(V6) { // Listen for changes on Virtual Pin V6
    int pinValue = param.asInt();
    if (pinValue == 1) {
        digitalWrite(relaypin, HIGH);
    } else {
        digitalWrite(relaypin, LOW);
    }
    Serial.print("V6 button value is: ");
    Serial.println(pinValue);
}
