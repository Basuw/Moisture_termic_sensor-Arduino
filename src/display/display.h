#ifndef DISPLAY_H
#define DISPLAY_H

#include <U8g2lib.h>

struct sensor {
    String name;
    float temperature, humidity;
};

void displayTempAndHumidity(bool print, sensor sens);
void displayScreen(U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C screen, sensor indoor, sensor outdoor);

#endif // DISPLAY_H
