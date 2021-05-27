
#include <WiFi.h>
#include <PubSubClient.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

char ssid[] = "shubham ext";          //  Change this to your network SSID (name).
char pass[] = "god1314@";           // Change this to your network password.
char mqttUserName[] = "TSesp32MQTTDemo";  // Use any name.
char mqttPass[] = "WP6S338XQVQZM2F1";      // Change to your MQTT API Key from Account > MyProfile.   
char writeAPIKey[] = "BYYMP3T3Q7YH4YY4";    // Change to your channel write API key.
long channelID = 1363280;                    // Change to your channel ID.
                       


static const char alphanum[] ="0123456789"
                              "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                              "abcdefghijklmnopqrstuvwxyz";  // For random generation of client ID



WiFiClient client;                           // Initialize the Wi-Fi client library.

PubSubClient mqttClient(client); // Initialize the PubSubClient library.
const char* server = "mqtt.thingspeak.com"; 

unsigned long lastConnectionTime = 0; 
const unsigned long postingInterval = 20L * 1000L; // Post data every 20 seconds.

void setup() {
  
  Serial.begin(9600);
  bme.begin(0x76);
  int status = WL_IDLE_STATUS;  // Set temporary Wi-Fi status.

  // Attempt to connect to Wi-Fi network.
  while (status != WL_CONNECTED) 
  {
    status = WiFi.begin(ssid, pass); // Connect to WPA/WPA2 Wi-Fi network.
    delay(5000);
  }

  Serial.println("Connected to wifi");
  mqttClient.setServer(server, 1883);   // Set the MQTT broker details.
}


    void reconnect() 
{
  char clientID[9];

  // Loop until reconnected.
  while (!mqttClient.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Generate ClientID
    for (int i = 0; i < 8; i++) {
        clientID[i] = alphanum[random(51)];
    }
    clientID[8]='\0';

    // Connect to the MQTT broker.
    if (mqttClient.connect(clientID,mqttUserName,mqttPass)) 
    {
      Serial.println("connected");
    } else 
    {
      Serial.print("failed, rc=");
      // Print reason the connection failed.
      // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void mqttPublishFeed() {

  float h = bme.readHumidity();  // Read humidity from DHT sensor.
  float t = bme.readTemperature();// Read temperature from DHT sensor.
  float p = bme.readPressure()/ 100.0F;
  if (isnan(h) || isnan(t)) {       // Condition to check whether the sensor reading was successful or not
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  
  // Create data string to send to ThingSpeak.
  String data = String("field1=") + String(t, DEC) + "&field2=" + String(p, DEC) + "&field3=" + String(h,DEC);
  int length = data.length();
  const char *msgBuffer;
  msgBuffer=data.c_str();
  Serial.println(msgBuffer);
  
  // Create a topic string and publish data to ThingSpeak channel feed. 
  String topicString = "channels/" + String( channelID ) + "/publish/"+String(writeAPIKey);
  length = topicString.length();
  const char *topicBuffer;
  topicBuffer = topicString.c_str();
  mqttClient.publish( topicBuffer, msgBuffer );
  lastConnectionTime = millis();
}


void loop() {

 // Reconnect if MQTT client is not connected.
  if (!mqttClient.connected()) 
  {
   reconnect();
  }

  mqttClient.loop();   // Call the loop continuously to establish connection to the server.

  // If interval time has passed since the last connection, publish data to ThingSpeak.
  if (millis() - lastConnectionTime > postingInterval) 
  {
    mqttPublishFeed(); // Publish three values simultaneously.
    // mqttPublishField(fieldNumber); // Use this function to publish to a single field
  }
}
