#include <SimpleDHT.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <stdio.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>


U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

//-- PIN --//
int pinDHT11Inside = 2; // pin inside temperature and humidity sensor
int pinDHT11Outside = 3; // pin Outside temperature and humidity sensor

int humidityFicus = 400; // humidity level for the ficus
int delayTime = 30; //seconds

int moistureOut=4; // pin for the moisture sensor
int pump=23; // pin for the pump

int moist=0;
int temp=0;
float tension = 3.5;

bool serialMonitorDisplay=false;

SimpleDHT11 inside(pinDHT11Inside);
SimpleDHT11 outside(pinDHT11Outside);

//-- WIFI --//

// Remplacez par vos informations de réseau
const char* ssid = "votre_SSID";
const char* password = "votre_mot_de_passe";

// Définir le port du serveur
WiFiServer server(80);


// termic sensor struct
struct Sensor {
  float temperature;
  float humidity;
};

void setup() {
  Serial.begin(9600);

  pinMode(A2,INPUT);  //temperature sensor
  pinMode(A3,INPUT);  //temperature sensor

  pinMode(pump,OUTPUT);

  digitalWrite(pump,HIGH);
  digitalWrite(moistureOut,HIGH);

  u8g2.begin();
}
void loop() {
  Serial.flush();
  u8g2.clearBuffer();					// clear the internal memory
  hydricSensor();
  sensor tempInside,tempOutside;
  tempInside=getTemp(inside);
  tempOutside=getTemp(outside);
  display(tempInside.temp,tempOutside.temp,tempInside.humidity,tempOutside.humidity);
  delay(delayTime*1000);
}

void wifiConnection(){
  Serial.print("Connecting to ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  // Démarrer le serveur
  server.begin();
  Serial.println("Server started");
}

void hydricSensor(){
  moist=analogRead(A0);
  temp=analogRead(A2);
  if(moist>humidityFicus){
    digitalWrite(pump,LOW);
    delay(2000);
    digitalWrite(pump,HIGH);
  }
  if(serialMonitorDisplay){
    Serial.println(moist);
  }
}


sensor getTemp(SimpleDHT11 dht11){
  sensor temp;
  sensor error;
  error.temp=0;
  error.humidity=0;
  int err = SimpleDHTErrSuccess;
  if((err=dht11.read2(&temp.temp, &temp.humidity, NULL)) !=  SimpleDHTErrSuccess){
    if(serialMonitorDisplay){
      Serial.print("Read DHT11 failed, err=");
      Serial.println(err);
    }
    return error;
  }
  if(serialMonitorDisplay){
    Serial.print(temp.temp);
    Serial.print(" *C, ");
    Serial.print(temp.humidity);
    Serial.println(" RH%");
  }
  return temp;
}

void wifiHandler(){
  // Vérifier les connexions entrantes
  WiFiClient client = server.available();
  if (client) {
    Serial.println("New client");
    String currentLine = "";
    String termicNumberParam = "";

    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Fin de la requête HTTP, envoyer la réponse
            if (termicNumberParam.length() > 0) {
              // Créer un objet JSON
              StaticJsonDocument<200> jsonDoc;
              jsonDoc["termicNumber"] = sensor.termicNumber;
              jsonDoc["temperature"] = sensor.temperature;
              jsonDoc["humidity"] = sensor.humidity;

              // Convertir l'objet JSON en chaîne
              String jsonResponse;
              serializeJson(jsonDoc, jsonResponse);

              // Envoyer l'en-tête HTTP
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: application/json");
              client.println("Connection: close");
              client.println();
              client.println(jsonResponse);
            } else {
              // Envoyer une réponse d'erreur
              client.println("HTTP/1.1 400 Bad Request");
              client.println("Content-Type: text/plain");
              client.println("Connection: close");
              client.println();
              client.println("Missing termicNumber parameter");
            }
            break;
          } else {
            // Vérifier si la ligne contient le paramètre termicNumber
            if (currentLine.startsWith("GET /?termicNumber=")) {
              int startIndex = currentLine.indexOf('=') + 1;
              int endIndex = currentLine.indexOf(' ', startIndex);
              termicNumberParam = currentLine.substring(startIndex, endIndex);
            }
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
}

void display(float tempIn, float tempOut, float humidityIn, float humidityOut){
  u8g2.setFont(u8g2_font_unifont_t_symbols);//utf8
  //Temperature
  char charTempIn[25];
  char charTempOut[25];

  String floatIn = String(tempIn,0);
  String strIn = "☁️ "+floatIn+"°C";
  String floatOut = String(tempOut,0);
  String strOut = "□ "+floatOut+"°C";
  
  strIn.toCharArray(charTempIn,20);
  strOut.toCharArray(charTempOut,20);

  //humidity
  char charHumiIn[25];
  char charHumiOut[25];

  String floatInH = String(humidityIn,0);
  String strInH = "¿ "+floatInH+"%";
  String floatOutH = String(humidityOut,0);
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