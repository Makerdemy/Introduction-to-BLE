#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTESP32.h>

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "85205650-5f30-11eb-8779-7d56e82df461";
char password[] = "f551c81964e7e1a3e4db09bbdd2efabfce599123";
char clientID[] = "b6bfee10-8afe-11eb-883c-638d8ce4c23d";

char ssid[] = "shubham";
char wifiPassword[] = "god1314@";

#define SENSOR_PIN 4 // Do not use digital pins 0 or 1 since those conflict with the use of Serial.
#define VIRTUAL_CHANNEL 1

void setup()
{
  pinMode(SENSOR_PIN, INPUT);
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop()
{
  Cayenne.loop();
  checkSensor();
}

int previousState = -1;
int currentState = -1;
unsigned long previousMillis = 0;

void checkSensor()
{
  unsigned long currentMillis = millis();
  // Check sensor data every 250 milliseconds
  if (currentMillis - previousMillis >= 250) {
    // Check the sensor state and send data when it changes.
    currentState = digitalRead(SENSOR_PIN);
    Serial.print("Motion:");
    Serial.println(currentState);
    if (currentState != previousState) {
      Cayenne.virtualWrite(0, currentState, "digital_sensor", "d");
      previousState = currentState;
    }
    previousMillis = currentMillis;
  }
}
