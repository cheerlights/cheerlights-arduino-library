#include "CheerLights.h"

CheerLights::CheerLights() {
    strncpy(_colorName, "black", sizeof(_colorName) - 1);
    _colorName[sizeof(_colorName) - 1] = '\0';
    _colorHex = 0x000000;
    _previousColorHex = 0x000000;
    _colorHue = 0;
    _colorSaturation = 0;
    _colorValue = 0;
    _colorChanged = false;
    _lastUpdate = 0;
    _hasFetched = false;
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
  // hasColorChanged() reports on this call only, also when it skips or fails
  _colorChanged = false;
  unsigned long currentTime = millis();

  // The first fetch always runs, even in the first seconds after boot
  if (_hasFetched && currentTime - _lastUpdate < MIN_UPDATE_INTERVAL) {
    return;
  }
  _hasFetched = true;
  _lastUpdate = currentTime;

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

  // Send the whole HTTP GET request in one write, without using String.
  // Separate print() calls go out as separate TCP segments, and every extra
  // segment is another chance to hit a lost packet and a retransmit (issue #2).
  char request[192];
  int requestLength = snprintf(request, sizeof(request),
                               "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
                               apiPath, host);
  if (requestLength <= 0 || requestLength >= (int)sizeof(request) ||
      client.write((const uint8_t*)request, requestLength) != (size_t)requestLength) {
    client.stop();
    return;
  }

  // Read the response a byte at a time as it arrives, within one deadline.
  // Stream timed reads are not used: the body has no trailing newline, so a
  // line read would wait out the stream timeout on every fetch.
  char line[BUFFER_SIZE];
  size_t lineLength = 0;
  bool statusLine = true;
  bool statusOk = false;
  bool inBody = false;
  long contentLength = -1;
  long bodyRead = 0;
  bool bodyDone = false;
  unsigned long started = millis();

  while (!bodyDone && millis() - started < TIMEOUT) {
    int c = client.read();
    if (c < 0) {
      if (!client.connected()) {
        bodyDone = inBody;  // the server closed the connection after the body
        break;
      }
      delay(1);
      continue;
    }

    if (inBody) {
      bodyRead++;
      if (c == '\n') {
        bodyDone = true;
      } else if (lineLength < sizeof(line) - 1) {
        line[lineLength++] = (char)c;
      }
      if (contentLength >= 0 && bodyRead >= contentLength) {
        bodyDone = true;
      }
      continue;
    }

    if (c != '\n') {
      if (lineLength < sizeof(line) - 1) {
        line[lineLength++] = (char)c;
      }
      continue;
    }

    // End of a status or header line
    if (lineLength > 0 && line[lineLength - 1] == '\r') lineLength--;
    line[lineLength] = 0;
    if (statusLine) {
      statusOk = strncmp(line, "HTTP/1.", 7) == 0 && strncmp(line + 8, " 200", 4) == 0;
      statusLine = false;
    } else if (lineLength == 0) {
      inBody = true;
      if (contentLength == 0) bodyDone = true;
    } else if (strncasecmp(line, "Content-Length:", 15) == 0) {
      contentLength = atol(line + 15);
    }
    lineLength = 0;
  }

  client.stop();

  if (!statusOk || !bodyDone) {
    return;
  }

  // Trim spaces and carriage returns around the color name
  line[lineLength] = 0;
  char* start = line;
  while (*start == ' ' || *start == '\r' || *start == '\t') start++;
  char* end = start + strlen(start);
  while (end > start && (end[-1] == ' ' || end[-1] == '\r' || end[-1] == '\t')) *--end = 0;
  if (*start == 0) {
    return;
  }

  strncpy(_colorName, start, sizeof(_colorName) - 1);
  _colorName[sizeof(_colorName) - 1] = 0;

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
