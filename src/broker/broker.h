#ifndef BROKER_H
#define BROKER_H

#include <WiFi.h>
#include <PubSubClient.h>
#include "../temp/thermalAndHumidity.h"

extern WiFiClient espClient;
extern PubSubClient client;

void callback(char* topic, byte* payload, unsigned int length);
void setup_wifi();
void reconnect();
void setupMQTT();
void loopMQTT();
void publishSensorData(sensor indoor, sensor outdoor);

#endif
