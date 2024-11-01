#ifndef CHEERLIGHTS_H
#define CHEERLIGHTS_H

#include <Arduino.h>

// Include the correct WiFi library based on the board
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266HTTPClient.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <HTTPClient.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  // For Arduino MKR1000 using WiFi101 library
  #include <WiFi101.h>
  #include <WiFiClient.h>
  #include <HTTPClient.h>
#elif defined(ARDUINO_SAMD_MKRWIFI1010)
  // For Arduino MKR WiFi 1010 using WiFiNINA library
  #include <WiFiNINA.h>
  #include <WiFiClient.h>
  #include <HTTPClient.h>
#elif defined(ARDUINO_AVR_UNO_WIFI_REV2)
  // For Arduino Uno WiFi Rev2
  #include <WiFiNINA.h>
  #include <WiFiClient.h>
  #include <HTTPClient.h>
#elif defined(ARDUINO_ARCH_SAMD)
  // For other SAMD boards
  #include <WiFiNINA.h>
  #include <WiFiClient.h>
  #include <HTTPClient.h>
#else
  // Default to WiFi.h and HTTPClient.h
  #include <WiFi.h>
  #include <HTTPClient.h>
#endif

class CheerLights {
  public:
    CheerLights();
    void begin(const char* ssid, const char* password);
    String getColorName();
    uint32_t getColorHex();
    uint8_t getRed();
    uint8_t getGreen();
    uint8_t getBlue();

  private:
    String _colorName;
    uint32_t _colorHex;
    void _fetchColor();
};

#endif // CHEERLIGHTS_H
