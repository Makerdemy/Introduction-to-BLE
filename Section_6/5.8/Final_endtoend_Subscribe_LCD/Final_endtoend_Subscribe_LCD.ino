#include <WiFi.h> //  ESP32
WiFiClient WIFI_CLIENT;

#include <PubSubClient.h>
PubSubClient MQTT_CLIENT;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 

const char * ssid = "shubham ext";
const char * password = "god1314@";

const char * mqttUserName = "shubhamg13114"; // Thingspeak username.
const char * mqttPass = "WP6S338XQVQZM2F1"; // Change to your MQTT API Key from Account > MyProfile.
const char * ChannelID = "1363280"; // Thingspeak Channel ID
const char * APIKey = "2HH1AOP0GILZGH8D"; // Thingspeak Channel Read API Key

static const char alphanum[] = "0123456789"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";

int intChar;
#define LED12 12 // LED12 ESP32

void setup() {
  pinMode(LED12, OUTPUT);
  digitalWrite(LED12, LOW);

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();

  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting with ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("WiFi conected. IP: ");
  Serial.println(WiFi.localIP());

  // Setting Callback.
  MQTT_CLIENT.setCallback(callback);
}

// What to do when it receives the data. 
void callback(char * received, byte * payload, unsigned int length) {
    Serial.println("Message received: ");
//     Serial.println(payload, int);
//     Serial.println(length);
//     Serial.println("   ");

    int intChar = atoi((char*)payload);
    Serial.println(intChar);
    
    if (intChar > 30){
      digitalWrite(LED12, HIGH);
     // Serial.println("greater than 30");
      lcd.setCursor(0, 0);
      lcd.print("Warning!");
      lcd.setCursor(0,1);
      lcd.print("Above limit");
    }
    
    else if(intChar < 30){
      digitalWrite(LED12, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Normal");
      Serial.println("less than 30");
    }
      Serial.println();
    }

    void loop() {
      if (!MQTT_CLIENT.connected()) {
        reconnect();
      }

      MQTT_CLIENT.loop(); // Check Subscription.

    }

    // Reconect MQTT broker
    void reconnect() {

      char clientID[9];

      MQTT_CLIENT.setServer("mqtt.thingspeak.com", 1883);
      //MQTT_CLIENT.setServer("mqtt.eclipse.org", 1883);
      MQTT_CLIENT.setClient(WIFI_CLIENT);

      // Trying connect with broker.
      while (!MQTT_CLIENT.connected()) {
        Serial.println("Trying to connect with Broker MQTT.");

        for (int i = 0; i < 8; i++) {
          clientID[i] = alphanum[random(51)];
        }
        clientID[8] = '\0';

        MQTT_CLIENT.connect(clientID, mqttUserName, mqttPass); // it isn't necessary..
        String topicString = "channels/" + String(ChannelID) + "/subscribe/fields/field1/" + String(APIKey);
        //length = topicString.length();
        const char * topicBuffer;
        topicBuffer = topicString.c_str();
        MQTT_CLIENT.subscribe(topicBuffer);

        // Wait to try to reconnect again...
        delay(3000);
      }

      Serial.println("Connected to MQTT.");
    }
