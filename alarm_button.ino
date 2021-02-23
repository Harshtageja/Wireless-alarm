 
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define Relay1            1
#define Relay2            3
#define Relay3            4
#define Relay4            13

#define WLAN_SSID       "Harsh"             // Your SSID
#define WLAN_PASS       "12345678"        // Your password

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "harshkumarhk0508"            // Replace it with your username
#define AIO_KEY         "aio_XTYU29qb2gopYmryKfvieVapJFph"   // Replace with your Project Auth Key

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay1");
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay 2");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/Relay4");



void MQTT_connect();

void setup() {
  Serial.begin(115200);

  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, INPUT);
  pinMode(Relay4, OUTPUT);
  
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
 

  // Setup MQTT subscription for onoff feed.
  
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light4);
}

void loop() {
 Serial.print(digitalRead(Relay3));
  MQTT_connect();
  while(1){
  if(!digitalRead(Relay3)){  
    
 Serial.print(digitalRead(Relay3));
  Serial.print(F("\nSending photocell val "));
  Serial.print("...");
  if (! photocell.publish(1)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  }
  delay(2000);
  
  
}}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
  
}
