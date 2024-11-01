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
- `getCurrentColor()`: Get the current CheerLights color from ThingSpeak.
- `showColorName()`: Show the current CheerLights color name (e.g. "red").
- `showColorHex()`: Show the current CheerLights color as a hex value (e.g. 0xFF0000).
- `showRed()`, `showGreen()`, `showBlue()`: Show the RGB values for the current CheerLights color (e.g. 255, 0, 0).