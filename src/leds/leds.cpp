#include <Adafruit_NeoPixel.h>

#define LED_WS2812B8_PIN 25
#define NUMPIXELS 8

Adafruit_NeoPixel pixels(NUMPIXELS, LED_WS2812B8_PIN, NEO_GRB + NEO_KHZ800);

void setupLED() {
    pixels.begin();
}

void setLEDColor(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
    pixels.setBrightness(brightness);
    for (int i = 0; i < NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(r, g, b));
    }
    pixels.show();
}

