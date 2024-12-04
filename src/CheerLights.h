#ifndef CHEERLIGHTS_H
#define CHEERLIGHTS_H

#include <Arduino.h>
#include <WiFiClient.h>

#define MIN_UPDATE_INTERVAL 5000
#define TIMEOUT             5000
#define BUFFER_SIZE         128

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

class CheerLights {
  public:
    CheerLights();
    void begin(const char* ssid, const char* password);
    void begin();
    bool reconnect();
    const char* getCurrentColor();
    const char* currentColorName();
    uint32_t currentColorHex();
    uint8_t currentRed();
    uint8_t currentGreen();
    uint8_t currentBlue();
    uint16_t currentHue();
    uint8_t currentSaturation();
    uint8_t currentValue();
    bool isConnected();
    bool hasColorChanged();
  private:
    void _connectToWiFi();
    void _fetchColor();
    const char* _ssid;
    const char* _password;
    char _colorName[32];
    uint32_t _colorHex;
    uint32_t _previousColorHex;
    uint16_t _colorHue;
    uint8_t _colorSaturation;
    uint8_t _colorValue;
    bool _colorChanged;
};

#endif // CHEERLIGHTS_H
