/*
'CheerLights_NeoPixel_Strip' example demonstrates how to use the CheerLights Arduino library to fetch and display the current CheerLights color on an Adafruit NeoPixel LED strip.

To join the CheerLights community, visit https://cheerlights.com.
*/

// Include the Adafruit NeoPixel library and initialize the strip
#include <Adafruit_NeoPixel.h>

#define LED_PIN    6
#define NUM_LEDS   8

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Include the correct WiFi library based on the board
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32)
  #include <WiFi.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  // For Arduino MKR1000 using WiFi101 library
  #include <WiFi101.h>
#elif defined(ARDUINO_SAMD_MKRWIFI1010)
  // For Arduino MKR WiFi 1010 using WiFiNINA library
  #include <WiFiNINA.h>
#elif defined(ARDUINO_AVR_UNO_WIFI_REV2)
  // For Arduino Uno WiFi Rev2
  #include <WiFiNINA.h>
#elif defined(ARDUINO_ARCH_SAMD)
  // For other SAMD boards
  #include <WiFiNINA.h>
#else
  #include <WiFi.h>
#endif

// Include the CheerLights library and instantiate the CheerLights object
#include <CheerLights.h>
CheerLights CheerLights;

// Include the secrets file that contains the WiFi credentials
#include "secrets.h"

unsigned long previousMillis = 0;
const long updateInterval = 15000;

void setup() {
    // Initialize serial communication
    Serial.begin(115200); 

    // Initialize the CheerLights library
    CheerLights.begin(SECRET_SSID, SECRET_PASSWORD);

    // Initialize the NeoPixel strip
    strip.begin();

    // Get the current CheerLights color and set the LEDs to that color
    CheerLights.getCurrentColor();
    Serial.print("Current CheerLights Color: ");
    Serial.println(CheerLights.currentColorName());
    Serial.println("--------------------------------");
    setLEDColors(CheerLights.currentRed(), CheerLights.currentGreen(), CheerLights.currentBlue());
}

void loop() {
    unsigned long currentMillis = millis();

    // Update the LEDs every updateInterval milliseconds
    if (currentMillis - previousMillis >= updateInterval) {
        previousMillis = currentMillis;

        // Get the current CheerLights color and set the LEDs to that color
        CheerLights.getCurrentColor();
        Serial.print("Current CheerLights Color: ");
        Serial.println(CheerLights.currentColorName());
        Serial.println("--------------------------------");
        setLEDColors(CheerLights.currentRed(), CheerLights.currentGreen(), CheerLights.currentBlue());
    }
}

void setLEDColors(uint8_t red, uint8_t green, uint8_t blue) {    
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();
}