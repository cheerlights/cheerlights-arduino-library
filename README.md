# CheerLights Arduino Library

An Arduino library to synchronize with the global CheerLights color by fetching the latest color from the CheerLights API.

## Features

- Fetch the latest CheerLights color name.
- Retrieve RGB values for the current color.
- Easy integration with LED strips like NeoPixels.

## IDE Installation

1. Open the Arduino IDE.
2. Select Tools > Manage Libraries.
3. Search for "CheerLights" and install.

## Usage

Include the library in your sketch and initialize it with your WiFi credentials. Use the provided methods to fetch and use the current CheerLights color.

## Methods

- `begin(const char* ssid, const char* password)`: Initialize the library with your WiFi credentials.
- `getColorName()`: Get the current CheerLights color name.
- `getColorHex()`: Get the current CheerLights color as a hex value.
- `getRed()`, `getGreen()`, `getBlue()`: Get the RGB values for the current CheerLights color.

## Example

```cpp
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
```

## License

This project is licensed under the MIT License.
