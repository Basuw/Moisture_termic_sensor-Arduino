#include <DHT.h>
#include <DHT_U.h>
#include "display/display.h"

#define SOIL_MOISTURE_PIN 34
#define DHT_IN_PIN 13
#define DHT_OUT_PIN 12

#define DHTTYPE DHT11     // Sensor type
extern DHT_Unified dhtIn;
extern DHT_Unified dhtOut;

sensor getTempAndHumidity(DHT_Unified dht, String name);

int soilHumidity();