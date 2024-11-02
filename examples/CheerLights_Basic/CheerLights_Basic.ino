/*
'CheerLights_Basic' example demonstrates how to use the CheerLights Arduino library to fetch and print the current CheerLights color.

To join the CheerLights community, visit https://cheerlights.com.
*/

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

    // Get the current CheerLights color and print it
    CheerLights.getCurrentColor();
    Serial.print("Current CheerLights Color: ");
    Serial.println(CheerLights.currentColorName());
}

void loop() {
    unsigned long currentMillis = millis();

    // Update the LEDs every updateInterval milliseconds
    if (currentMillis - previousMillis >= updateInterval) {
        previousMillis = currentMillis;

        // Get the current CheerLights color and print it
        CheerLights.getCurrentColor();
        Serial.print("Current CheerLights Color: ");
        Serial.println(CheerLights.currentColorName());

        // Print the current CheerLights color as a hex value
        Serial.print("Current CheerLights Color Hex: ");
        Serial.println(CheerLights.currentColorHex());

        // Print the current CheerLights color as RGB values
        Serial.print("Current CheerLights Color RGB: ");
        Serial.print(CheerLights.currentRed());
        Serial.print(", ");
        Serial.print(CheerLights.currentGreen());
        Serial.print(", ");
        Serial.println(CheerLights.currentBlue());

        // Check if the color has changed
        if (CheerLights.hasColorChanged()) {
            Serial.println("Color has changed!");
        }
        else {
            Serial.println("Color has not changed.");
        }
        Serial.println("--------------------------------");
    }
} 