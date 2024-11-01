/*
'CheerLightsExample' demonstrates how to use the CheerLights library to fetch and display the current CheerLights color on an Adafruit NeoPixel LED strip.

To learn more about CheerLights, visit https://cheerlights.com.
*/

#include <CheerLights.h>
#include "secrets.h"
#include <Adafruit_NeoPixel.h> // For controlling NeoPixels

#define LED_PIN    6
#define NUM_LEDS   8

CheerLights CheerLights;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

unsigned long previousMillis = 0;
const long interval = 15000; // 15 seconds

void setup() {
    Serial.begin(115200);
    CheerLights.begin(SECRET_SSID, SECRET_PASSWORD);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Get the current CheerLights color
        Serial.print("Current CheerLights Color: ");
        Serial.println(CheerLights.getCurrentColor());

        // Show the color as a hex value
        Serial.print("Color Hex: 0x");
        Serial.println(CheerLights.showColorHex(), HEX);

        // Show the RGB values
        uint8_t red = CheerLights.showRed();
        uint8_t green = CheerLights.showGreen();
        uint8_t blue = CheerLights.showBlue();

        Serial.print("Red: ");
        Serial.print(red);
        Serial.print(" Green: ");
        Serial.print(green);
        Serial.print(" Blue: ");
        Serial.println(blue);

        // Show the color name
        Serial.print("CheerLights Color Name: ");
        Serial.println(CheerLights.showColorName());

        // Set all pixels to the CheerLights color using the RGB values
        for (int i = 0; i < NUM_LEDS; i++) {
            strip.setPixelColor(i, strip.Color(red, green, blue));
        }
        strip.show();
    }

    // Other non-blocking code can go here
}
