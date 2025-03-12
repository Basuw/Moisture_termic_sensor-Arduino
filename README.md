# Moisture and Temperature Sensor with Arduino

This project is an Arduino-based system that monitors soil moisture and temperature, and controls a water pump based on the soil moisture level. It also displays the temperature and humidity readings on an OLED screen.

## Components

- ESP32 Development Board
- DHT11 Temperature and Humidity Sensor (x2)
- Soil Moisture Sensor
- Water Pump
- OLED Display (SSD1306)
- LED
- Resistors, Wires, and Breadboard

## Libraries

The following libraries are required for this project:

- Adafruit Sensor Library
- DHT Sensor Library
- U8g2 Library

These libraries can be installed via PlatformIO or the Arduino Library Manager.

## Setup

1. Connect the components as per the following pin configuration:

   - DHT11 Indoor Sensor: Pin 13
   - DHT11 Outdoor Sensor: Pin 12
   - Soil Moisture Sensor: Pin 34
   - Water Pump: Pin 32
   - LED: Pin 2
   - OLED Display: SCL (Pin 22), SDA (Pin 21)

2. Clone this repository and open it in PlatformIO.

3. Ensure the `platformio.ini` file is configured correctly:

   ```ini
   [env:esp32dev]
   platform = espressif32
   board = esp32dev
   framework = arduino
   lib_deps = adafruit/DHT sensor library@^1.4.6

## Authors :

Bastien Jacquelin
