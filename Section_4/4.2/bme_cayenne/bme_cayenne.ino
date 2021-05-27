/*
This example shows how to connect to Cayenne using an ESP32 and send/receive sample data.

The CayenneMQTT Library is required to run this sketch. If you have not already done so you can install it from the Arduino IDE Library Manager.

Steps:
1. If you have not already installed the ESP32 Board Package install it using the instructions here: https://github.com/espressif/arduino-esp32/blob/master/README.md#installation-instructions.
2. Select your ESP32 board from the Tools menu.
3. Set the Cayenne authentication info to match the authentication info from the Dashboard.
4. Set the network name and password.
5. Compile and upload the sketch.
6. A temporary widget will be automatically generated in the Cayenne Dashboard. To make the widget permanent click the plus sign on the widget.
*/

#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

// WiFi network info.
char ssid[] = "shubham";
char wifiPassword[] = "god1314@";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "85205650-5f30-11eb-8779-7d56e82df461";
char password[] = "f551c81964e7e1a3e4db09bbdd2efabfce599123";
char clientID[] = "b6bfee10-8afe-11eb-883c-638d8ce4c23d";

void setup() {
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  bme.begin(0x76);
}

void loop() {
  Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
  // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
  //Cayenne.virtualWrite(0, millis());

  Cayenne.celsiusWrite(1, bme.readTemperature());
  Cayenne.virtualWrite(2, bme.readPressure()/ 100.0F, "bp", "pa");
  Cayenne.virtualWrite(3, bme.readHumidity(), "rel_hum", "p");
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
