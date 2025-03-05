#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <U8g2lib.h>

#define DHT_IN_PIN 13
#define DHT_OUT_PIN 12

#define SOIL_MOISTURE_PIN 34
#define PUMP_PIN 32
#define HUMIDITY_THRESHOLD 35

#define DHTTYPE DHT11     // Sensor type

#define PRINT_MONITOR true

#define LED 2

DHT_Unified dhtIn(DHT_IN_PIN, DHTTYPE);
DHT_Unified dhtOut(DHT_OUT_PIN, DHTTYPE);

uint32_t delayMS;

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(
  U8G2_R0,   // Rotation normale
  /* clock=*/ 22,  // SCL (SCK)
  /* data=*/ 21,   // SDA
  /* reset=*/ U8X8_PIN_NONE
);

struct sensor{
  String name;
  float temperature, humidity;
};

void setup() {
  Serial.begin(9600);
  //PIN CONFIGURATION
  pinMode(LED, OUTPUT);
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(PUMP_PIN, OUTPUT);

  dhtIn.begin();
  u8g2.begin();
  u8g2.enableUTF8Print();  // Pour utiliser les caractères UTF-8
}



void displayTempAndHumidity(sensor sens) {
  if(PRINT_MONITOR){
    Serial.print(sens.name);
    Serial.print(" : ");
    Serial.print(sens.temperature);
    Serial.print("°C, ");
    Serial.print(sens.humidity);
    Serial.println("%");
  }
}

sensor tempAndHumidty(DHT_Unified dht, String name) {
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

void pump(int humPercent){
  if(humPercent < HUMIDITY_THRESHOLD) {
    digitalWrite(LED, HIGH);
    digitalWrite(PUMP_PIN, LOW);
    delay(1000);
    digitalWrite(LED, LOW);
    digitalWrite(PUMP_PIN, HIGH);
  }
}

void displayScreen(sensor indoor, sensor outdoor){
  u8g2.setFont(u8g2_font_unifont_t_symbols);//utf8
  //Temperature
  char charTempIn[25];
  char charTempOut[25];

  String floatIn = String(indoor.temperature,0);
  String strIn = "☁️ "+floatIn+"°C";
  String floatOut = String(outdoor.temperature,0);
  String strOut = "□ "+floatOut+"°C";
  
  strIn.toCharArray(charTempIn,20);
  strOut.toCharArray(charTempOut,20);

  //humidity
  char charHumiIn[25];
  char charHumiOut[25];

  String floatInH = String(indoor.humidity,0);
  String strInH = "¿ "+floatInH+"%";
  String floatOutH = String(outdoor.humidity,0);
  String strOutH = "¿ "+floatOutH+"%";
  
  strInH.toCharArray(charHumiIn,20);
  strOutH.toCharArray(charHumiOut,20);


  //display
  u8g2.drawUTF8(5, 10, charTempIn);
  u8g2.drawUTF8(5, 30, charTempOut);
  u8g2.drawUTF8(87, 10, charHumiIn);
  u8g2.drawUTF8(87, 30, charHumiOut);
  u8g2.sendBuffer();
}

void loop() {
	digitalWrite(LED, LOW);
	delay(500);
  // Get temperature event and print its value.
  digitalWrite(PUMP_PIN, HIGH);
  sensor indoor = tempAndHumidty(dhtIn, "Indoor");
  displayTempAndHumidity(indoor);
  sensor outdoor = tempAndHumidty(dhtOut, "Outdoor");
  displayTempAndHumidity(outdoor);
  displayScreen(indoor, outdoor);
  int humPercent = soilHumidity();
  pump(humPercent);
	delay(500);
}