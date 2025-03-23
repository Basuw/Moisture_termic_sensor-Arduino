#include "temp/thermalAndHumidity.h"
#include "display/display.h"
#include "leds/leds.h"
#include "broker/broker.h"  // Include the broker header file

#define PUMP_PIN 32
#define HUMIDITY_THRESHOLD 35

#define I2C_1_PIN 22
#define I2C_2_PIN 21

#define PRINT_MONITOR true
#define LED 2

uint32_t delayMS;
unsigned long lastPublishTime = 0;  // Add a variable to track the last publish time
const unsigned long publishInterval = 60000;  // 1 minute interval

DHT_Unified dhtIn(DHT_IN_PIN, DHTTYPE);
DHT_Unified dhtOut(DHT_OUT_PIN, DHTTYPE);

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE, I2C_1_PIN, I2C_2_PIN); 

void setup() {
  Serial.begin(9600);
  //PIN CONFIGURATION
  pinMode(LED, OUTPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);

  dhtIn.begin();
  u8g2.begin();
  u8g2.enableUTF8Print();  // Pour utiliser les caractères UTF-8

  setupLED(); // Initialize the WS2812B8 LED
  setupMQTT(); // Initialize MQTT
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
  sensor indoor = getTempAndHumidity(dhtIn, "Indoor");
  displayTempAndHumidity(PRINT_MONITOR,indoor);
  sensor outdoor = getTempAndHumidity(dhtOut, "Outdoor");
  displayTempAndHumidity(PRINT_MONITOR,outdoor);
  displayScreen(u8g2,indoor, outdoor);
  int humPercent = soilHumidity();
  pump(humPercent);

  // Set LED color and intensity
  setLEDColor(255, 0, 0, 50); // Example: Red color with 50% brightness

  unsigned long currentTime = millis();
  if (currentTime - lastPublishTime >= publishInterval) {
    publishSensorData(indoor, outdoor); // Publish sensor data to MQTT topic
    lastPublishTime = currentTime;  // Update the last publish time
  }

  loopMQTT(); // Handle MQTT loop

  delay(500);
}