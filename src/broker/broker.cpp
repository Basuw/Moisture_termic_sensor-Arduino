#include <WiFi.h>
#include <PubSubClient.h>
#include "broker.h"
#include "WiFiConfig/WiFiConfig.h"
#include "MQTTConfig/MQTTConfig.h"
#include "../leds/leds.h"
#include "../temp/thermalAndHumidity.h"

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (String(topic) == MQTT_LED_TOPIC) {
    int red, green, blue, brightness;
    sscanf(message.c_str(), "%d,%d,%d,%d", &red, &green, &blue, &brightness);
    setLEDColor(red, green, blue, brightness);
  }
}

void setup_wifi() {
  unsigned long startAttemptTime = millis();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
  } else {
    Serial.println("WiFi connection failed");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client", MQTT_USER, MQTT_PASSWORD)) {
      Serial.println("connected");
      client.subscribe(MQTT_LED_TOPIC);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      unsigned long retryStartTime = millis();
      while (millis() - retryStartTime < 5000) {
        // Wait for 5 seconds before retrying
      }
    }
  }
}

void setupMQTT() {
  client.setServer(MQTT_BROKER_IP, MQTT_BROKER_PORT);
  client.setCallback(callback);
  setup_wifi();
  reconnect();
}

void loopMQTT() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void publishSensorData(sensor indoor, sensor outdoor) {
  String payload = String("Indoor: ") + indoor.temperature + "," + indoor.humidity + " Outdoor: " + outdoor.temperature + "," + outdoor.humidity;
  client.publish(MQTT_TOPIC, payload.c_str());
}
