#ifndef CHEERLIGHTS_H
#define CHEERLIGHTS_H

#include <Arduino.h>
#include <WiFiClient.h>

class CheerLights {
  public:
    CheerLights();
    void begin(const char* ssid, const char* password);
    String getCurrentColor();
    String currentColorName();
    uint32_t currentColorHex();
    uint8_t currentRed();
    uint8_t currentGreen();
    uint8_t currentBlue();

  private:
    void _connectToWiFi();
    void _fetchColor();
    const char* _ssid;
    const char* _password;
    String _colorName;
    uint32_t _colorHex;
};

#endif // CHEERLIGHTS_H
