#include <SimpleDHT.h>
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include <Wire.h>
#include <stdio.h>

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0); 

int pinDHT11Inside = 48;
int pinDHT11Outside = 46;

int humidityFicus = 450;
int delayTime = 30; //seconds

SimpleDHT11 inside(pinDHT11Inside);
SimpleDHT11 outside(pinDHT11Outside);

int moistureOut=22;
int pump=52;
int moist=0;
int temp=0;
float tension = 3.5;


struct sensor{
  float temp, humidity;
};

void setup() {
  Serial.begin(9600);

  pinMode(A0,INPUT);  
  pinMode(A2,INPUT);  

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


void hydricSensor(){
  moist=analogRead(A0);
  temp=analogRead(A2);
  if(moist>humidityFicus){
    digitalWrite(pump,LOW);
    delay(2000);
    digitalWrite(pump,HIGH);
  }
  else{
    digitalWrite(pump,HIGH);
  }
  Serial.println(moist);
}


sensor getTemp(SimpleDHT11 dht11){
  sensor temp;
  sensor error;
  error.temp=0;
  error.humidity=0;
  int err = SimpleDHTErrSuccess;
  if((err=dht11.read2(&temp.temp, &temp.humidity, NULL)) !=  SimpleDHTErrSuccess){
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    return error;
  }

  Serial.print(temp.temp);
  Serial.print(" *C, ");
  Serial.print(temp.humidity);
  Serial.println(" RH%");

  return temp;
}


void display(float tempIn, float tempOut, float humidityIn, float humidityOut){
  u8g2.setFont(u8g2_font_unifont_t_symbols);//utf8
  //Temperature
  char charTempIn[25];
  char charTempOut[25];

  String floatIn = String(tempIn,0);
  String strIn = "☁️ "+floatIn+"°C";
  String floatOut = String(tempOut,0);
  String strOut = "♥ "+floatOut+"°C";
  
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