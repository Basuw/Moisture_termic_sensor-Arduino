#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHT_IN_PIN 13
#define DHT_OUT_PIN 12

#define SOIL_MOISTURE_PIN 34
#define PUMP_PIN 32
#define HUMIDITY_THRESHOLD 35

#define DHTTYPE DHT11     // Sensor type

#define LED 2

DHT_Unified dhtIn(DHT_IN_PIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  Serial.begin(9600);
  //PIN CONFIGURATION
  pinMode(LED, OUTPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);

  dhtIn.begin();
}



void displayTempAndHumidity(String name, float temperature, float humidity) {
  Serial.print(name);
  Serial.print(" : ");
  Serial.print(temperature);
  Serial.print("°C, ");
  Serial.print(humidity);
  Serial.println("%");

}

void tempAndHumidty(DHT_Unified dht, String name) {
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float temp, hum;
  if (isnan(event.temperature)) {
    Serial.print(F("Error reading temperature from "));
    Serial.println(name);
    return;
  }
  temp = event.temperature;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.print(F("Error reading humidity from"));
    Serial.println(name);
    return;
  }
  hum = event.relative_humidity;
  displayTempAndHumidity(name, temp, hum);
}


int soilHumidity() {
  int sensorValue = analogRead(SOIL_MOISTURE_PIN);
  float moisturePercent = map(sensorValue, 4095, 0, 0, 100);

  Serial.print("Soil humidity : ");
  Serial.print(moisturePercent);
  Serial.println(" %");

  return moisturePercent;
}

void pump(int humPercent){
  if(humPercent < HUMIDITY_THRESHOLD) {
    digitalWrite(LED, HIGH);
    digitalWrite(PUMP_PIN, LOW);
    delay(1000);
    digitalWrite(LED, LOW);
    digitalWrite(PUMP_PIN, HIGH);
  }
}


void loop() {
	digitalWrite(LED, LOW);
	delay(500);
  // Get temperature event and print its value.
  digitalWrite(PUMP_PIN, HIGH);
  tempAndHumidty(dhtIn, "Indoor");
  int humPercent = soilHumidity();
  pump(humPercent);
	delay(500);
}