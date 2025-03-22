#include "thermalAndHumidity.h"
#include "display/display.h"
#include <DHT.h>
#include <DHT_U.h>

sensor getTempAndHumidity(DHT_Unified dht, String name){
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    sensor sens;
    sens.name = name;
    if (isnan(event.temperature)) {
      Serial.print(F("Error reading temperature from "));
      Serial.println(sens.name);
      return sens;
    }
    sens.temperature = event.temperature;
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.print(F("Error reading humidity from"));
      Serial.println(sens.name);
      return sens;
    }
    sens.humidity = event.relative_humidity;
    return sens;
}

int soilHumidity() {
    int sensorValue = analogRead(SOIL_MOISTURE_PIN);
    float moisturePercent = map(sensorValue, 4095, 0, 0, 100);
  
    Serial.print("Soil humidity : ");
    Serial.print(moisturePercent);
    Serial.println(" %");
  
    return moisturePercent;
}