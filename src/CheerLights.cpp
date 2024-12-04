#include "CheerLights.h"

CheerLights::CheerLights() {
    strncpy(_colorName, "black", sizeof(_colorName) - 1);
    _colorName[sizeof(_colorName) - 1] = '\0';
    _colorHex = 0x000000;
    _previousColorHex = 0x000000;
    _colorHue = 0;
    _colorSaturation = 0;
    _colorValue = 0;
}

void CheerLights::begin(const char* ssid, const char* password) {
  // Store WiFi credentials
  _ssid = ssid;
  _password = password;

  // Initial WiFi connection
  _connectToWiFi();
}

void CheerLights::begin() {
  _ssid = nullptr;
  _password = nullptr;
}

bool CheerLights::reconnect() {
  if (WiFi.status() == WL_CONNECTED) {
    return true;
  }
  
  _connectToWiFi();
  return WiFi.status() == WL_CONNECTED;
}

void CheerLights::_connectToWiFi() {
  if (_ssid == nullptr || _password == nullptr) {
    return;
  }

  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void CheerLights::_fetchColor() {
  static unsigned long lastUpdate = 0;
  unsigned long currentTime = millis();

  if (currentTime - lastUpdate < MIN_UPDATE_INTERVAL) {
    return;
  }
  lastUpdate = currentTime;

  // Check WiFi connection and attempt to reconnect if necessary
  if (_ssid != nullptr && _password != nullptr) {
    if (WiFi.status() != WL_CONNECTED) {
      _connectToWiFi();
      if (WiFi.status() != WL_CONNECTED) {
        return;
      }
    }
  }

  const char* host = "api.thingspeak.com";
  const int httpPort = 80;
  const char* apiPath = "/channels/1417/field/1/last.txt?headers=false";

  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    client.stop();
    return;
  }

  // Create the HTTP GET request without using String
  client.print("GET ");
  client.print(apiPath);
  client.print(" HTTP/1.1\r\nHost: ");
  client.print(host);
  client.print("\r\nConnection: close\r\n\r\n");

  // Wait for response
  unsigned long timeout = millis();
  while (client.connected() && !client.available()) {
    if (millis() - timeout > TIMEOUT) {
      client.stop();
      return;
    }
  }

  // Read the response
  bool headersEnd = false;
  char line[BUFFER_SIZE];
  while (client.connected() || client.available()) {
    int len = client.readBytesUntil('\n', line, sizeof(line) - 1);
    line[len] = 0;

    char* start = line;
    while (*start == ' ' || *start == '\r') start++;
    char* end = line + strlen(line) - 1;
    while (end > start && (*end == ' ' || *end == '\r')) *end-- = 0;

    if (!headersEnd) {
      if (strlen(start) == 0) {
        headersEnd = true;
      }
      continue;
    }

    if (strlen(start) > 0) {
      strncpy(_colorName, start, sizeof(_colorName) - 1);
      _colorName[sizeof(_colorName) - 1] = 0;
      break;
    }
  }

  client.stop();

  // Convert color name to lowercase
  for (char* p = _colorName; *p; ++p) *p = tolower(*p);

  // Map the color name to a hex value
  static const struct {
    const char* name;
    uint32_t color;
  } colorMap[] = {
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
    if (strcasecmp(_colorName, color.name) == 0) {
      _colorHex = color.color;      
      break;
    }
  }

  // Map the color name to hue, saturation, and value
  static const struct {
    const char* name;
    uint16_t hue;
    uint8_t saturation;
    uint8_t value;
  } colorMapHSV[] = {
    {"red", 0, 255, 255},
    {"green", 21845, 255, 255},
    {"blue", 43690, 255, 255},
    {"cyan", 32767, 255, 255},
    {"white", 0, 0, 255},
    {"warmwhite", 7123, 23, 253},
    {"oldlace", 7123, 23, 253},
    {"magenta", 54612, 255, 255},
    {"yellow", 10922, 255, 255},
    {"orange", 6954, 255, 255},
    {"purple", 54612, 255, 128},
    {"pink", 63627, 63, 255},
    {"black", 0, 0, 0}
  };

  for (const auto& colorHSV : colorMapHSV) {
    if (strcasecmp(_colorName, colorHSV.name) == 0) {
      _colorHue = colorHSV.hue;
      _colorSaturation = colorHSV.saturation;
      _colorValue = colorHSV.value;
      break;
    }
  }

  _colorChanged = _colorHex != _previousColorHex;
  _previousColorHex = _colorHex;
}

const char* CheerLights::getCurrentColor() {
  _fetchColor();
  return _colorName;
}

const char* CheerLights::currentColorName() {
  return _colorName;
}

uint32_t CheerLights::currentColorHex() {
  return _colorHex;
}

uint8_t CheerLights::currentRed() {
  return (_colorHex >> 16) & 0xFF;
}

uint8_t CheerLights::currentGreen() {
  return (_colorHex >> 8) & 0xFF;
}

uint8_t CheerLights::currentBlue() {
  return _colorHex & 0xFF;
}

uint16_t CheerLights::currentHue() {
  return _colorHue;
}

uint8_t CheerLights::currentSaturation() {
  return _colorSaturation;
}

uint8_t CheerLights::currentValue() {
  return _colorValue;
}

bool CheerLights::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}

bool CheerLights::hasColorChanged() {
  return _colorChanged;
}
