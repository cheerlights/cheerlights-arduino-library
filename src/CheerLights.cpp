#include "CheerLights.h"

#define MIN_UPDATE_INTERVAL 5000

CheerLights::CheerLights() {
  _colorName = "black";
  _colorHex = 0x000000;
}

void CheerLights::begin(const char* ssid, const char* password) {
  Serial.begin(115200);

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  
  #if defined(ESP8266) || defined(ESP32)
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  #elif defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_ARCH_SAMD)
    while (WiFi.begin(ssid, password) != WL_CONNECTED) {
      delay(5000);
      Serial.print(".");
    }
  #else
    // Default WiFi connection method
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  #endif

  Serial.println("\nConnected to WiFi");
}

void CheerLights::_fetchColor() {
  static unsigned long lastUpdate = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastUpdate < MIN_UPDATE_INTERVAL) {
    return;
  }
  lastUpdate = currentTime;

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi not connected"));
    return;
  }

  HTTPClient http;
  http.begin(F("http://api.thingspeak.com/channels/1417/field/1/last.txt"));
  int httpCode = http.GET();
  
  if (httpCode != HTTP_CODE_OK) {
    Serial.println(F("Failed to fetch color"));
    http.end();
    return;
  }

  _colorName = http.getString();
  http.end();

  _colorName.trim();
  _colorName.toLowerCase();

  // Use progmem for color mapping
  static const struct {
    const char* name;
    uint32_t color;
  } colorMap[] PROGMEM = {
    {"red", 0xFF0000},
    {"green", 0x00FF00},
    {"blue", 0x0000FF},
    {"cyan", 0x00FFFF},
    {"white", 0xFFFFFF},
    {"warmwhite", 0xFDF5E6},
    {"oldlace", 0xFDF5E6},
    {"magenta", 0xFF00FF},
    {"yellow", 0xFFFF00},
    {"orange", 0xFFA500},
    {"purple", 0x800080},
    {"pink", 0xFFC0CB},
    {"black", 0x000000}
  };

  _colorHex = 0x000000; // Default to black
  for (const auto& color : colorMap) {
    if (_colorName == FPSTR(color.name)) {
      _colorHex = pgm_read_dword(&color.color);
      break;
    }
  }
}

String CheerLights::getColorName() {
  _fetchColor();
  return _colorName;
}

uint32_t CheerLights::getColorHex() {
  _fetchColor();
  return _colorHex;
}

uint8_t CheerLights::getRed() {
  return (_colorHex >> 16) & 0xFF;
}

uint8_t CheerLights::getGreen() {
  return (_colorHex >> 8) & 0xFF;
}

uint8_t CheerLights::getBlue() {
  return _colorHex & 0xFF;
}
