# CheerLights Arduino Library

An Arduino library to synchronize with the global CheerLights color by fetching the latest color from the CheerLights API.

## Features

- Fetch the latest CheerLights color name.
- Retrieve RGB values for the current color.
- Retrieve HSV values for the current color.
- Easy integration with LED strips like NeoPixels.

## IDE Installation

1. Open the Arduino IDE.
2. Select Tools > Manage Libraries.
3. Search for "CheerLights" and install.

## Usage

Include the library in your sketch and initialize it with your WiFi credentials. Use the provided methods to fetch and use the current CheerLights color.

## Methods

- `begin(const char* ssid, const char* password)`: Initialize the library with your WiFi credentials to let the library handle the WiFi connection.
- `begin()`: Initialize the library without WiFi credentials (use this if you want to manually connect to the WiFi network and manage the WiFi connection outside of the library).
- `reconnect()`: Reconnect to the WiFi network.
- `isConnected()`: Returns a boolean indicating whether the library is connected to the WiFi network.
- `getCurrentColor()`: Get the current CheerLights color from ThingSpeak channel 1417. Returns a pointer to a constant char array.
- `currentColorName()`: The current CheerLights color name (e.g. "red"). Returns a pointer to a constant char array.
- `currentColorHex()`: The current CheerLights color as a hex value (e.g. 0xFF0000). Returns a uint32_t.
- `currentRed()`, `currentGreen()`, `currentBlue()`: The RGB values for the current CheerLights color (e.g. 255, 0, 0). Returns a uint8_t.
- `currentHue()`, `currentSaturation()`, `currentValue()`: The HSV values for the current CheerLights color (e.g. 0, 255, 255). Returns a uint16_t for hue and uint8_t for saturation and value.
- `hasColorChanged()`: Returns a boolean indicating whether the current CheerLights color has changed since the last call to this method.

## Compatibility

This library supports WiFi-enabled Arduino-compatible boards. It has been tested with the following boards:

- Arduino MKR1000
- Arduino MKR WiFi 1010
- Arduino Uno WiFi Rev2
- Arduino Mega WiFi Rev2
- Arduino Uno R4 WiFi
- ESP8266
- ESP32