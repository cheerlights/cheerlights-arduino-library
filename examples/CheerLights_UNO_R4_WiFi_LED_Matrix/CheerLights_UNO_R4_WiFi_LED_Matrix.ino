/*
'CheerLights_UNO_R4_WiFi_LED_Matrix' example demonstrates how to use the CheerLights Arduino library to fetch and display the current CheerLights color on an LED matrix on the Arduino Uno R4 WiFi.

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

// Include the ArduinoGraphics library and the Arduino LED Matrix library
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"

ArduinoLEDMatrix matrix;

byte cheerLightsIcon[8][12] = {
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
  { 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1 },
  { 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
  { 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1 },
  { 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1 },
  { 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 }
};

String currentColorName = "";
String text = "";

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Initialize the LED matrix
  matrix.begin();

  // Initialize the CheerLights library and connect to the WiFi network
  CheerLights.begin(SECRET_SSID, SECRET_PASSWORD);

  // Get the current CheerLights color and update the text
  CheerLights.getCurrentColor();
  currentColorName = CheerLights.currentColorName();
  text = "     Connecting to CheerLights     ";

  // Display the CheerLights icon for 5 seconds
  matrix.renderBitmap(cheerLightsIcon, 8, 12);
  delay(5000);
}

void loop() {
  unsigned long currentMillis = millis();

  // Update the CheerLights color every updateInterval milliseconds
  if (currentMillis - previousMillis >= updateInterval) {
    previousMillis = currentMillis;

    // Get the current CheerLights color and update the text if it has changed
    CheerLights.getCurrentColor();

    if (CheerLights.hasColorChanged()) {
      currentColorName = CheerLights.currentColorName();
      text = "    CheerLights: " + currentColorName + "    ";
      Serial.print("CheerLights: ");
      Serial.println(currentColorName);
    }
  }

  // Display the scrolling text with the current CheerLights color
  matrix.beginDraw();
  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(50);
  matrix.textFont(Font_5x7);
  matrix.beginText(0, 1, 0xFFFFFF);
  matrix.println(text.c_str());
  matrix.endText(SCROLL_LEFT);
  matrix.endDraw();
}
