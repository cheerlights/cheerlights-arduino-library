/*
'CheerLightsExample' demonstrates how to use the CheerLights library to fetch and display the current CheerLights color on an Adafruit NeoPixel LED strip.

To learn more about CheerLights, visit https://cheerlights.com.
*/

#include <CheerLights.h>
#include <WiFiClientSecure.h>
#include <Adafruit_NeoPixel.h> // For controlling NeoPixels

#define LED_PIN    6
#define NUM_LEDS   8

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

CheerLights cheerLights;
Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    Serial.begin(115200);
    cheerLights.begin(ssid, password);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'
}

void loop() {
    // Fetch and update the color
    cheerLights.getColorHex();

    // Now get the RGB values
    uint8_t red = cheerLights.getRed();
    uint8_t green = cheerLights.getGreen();
    uint8_t blue = cheerLights.getBlue();

    Serial.print("Current CheerLights Color: ");
    Serial.println(cheerLights.getColorName());

    // Set all pixels to the CheerLights color
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(red, green, blue));
    }
    strip.show();

    delay(15000); // Update every 15 seconds
}
