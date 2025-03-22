#include "display.h"
#include <Adafruit_Sensor.h>
#include <U8g2lib.h>

void displayTempAndHumidity(bool print, sensor sens) {
    if(print){
      Serial.print(sens.name);
      Serial.print(" : ");
      Serial.print(sens.temperature);
      Serial.print("°C, ");
      Serial.print(sens.humidity);
      Serial.println("%");
    }
  }

void displayScreen(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C screen, sensor indoor, sensor outdoor){
    screen.setFont(u8g2_font_unifont_t_symbols);//utf8
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
    screen.drawUTF8(5, 10, charTempIn);
    screen.drawUTF8(5, 30, charTempOut);
    screen.drawUTF8(87, 10, charHumiIn);
    screen.drawUTF8(87, 30, charHumiOut);
    screen.sendBuffer();
}