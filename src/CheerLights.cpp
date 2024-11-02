#include "CheerLights.h"

#define MIN_UPDATE_INTERVAL 5000
#define TIMEOUT             5000

CheerLights::CheerLights() {
  _colorName = "black";
  _colorHex = 0x000000;
}

void CheerLights::begin(const char* ssid, const char* password) {
  Serial.begin(115200);

  // Store WiFi credentials
  _ssid = ssid;
  _password = password;

  // Initial WiFi connection
  _connectToWiFi();
}

void CheerLights::_connectToWiFi() {
  Serial.print("Connecting to WiFi");

  #if defined(ESP8266) || defined(ESP32) 
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  #elif defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_AVR_UNO_WIFI_REV2) || defined(ARDUINO_ARCH_SAMD)
    while (WiFi.begin(_ssid, _password) != WL_CONNECTED) {
      delay(5000);
      Serial.print(".");
    }
  #else
    // Default WiFi connection method
    WiFi.begin(_ssid, _password);
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
    Serial.println("Update interval not reached, skipping request.");
    return;
  }
  lastUpdate = currentTime;

  // Check WiFi connection and attempt to reconnect if necessary
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(F("WiFi not connected, attempting to reconnect..."));
    _connectToWiFi();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println(F("Failed to reconnect to WiFi."));
      return;
    }
  }

  const char* host = "api.thingspeak.com";
  const int httpPort = 80;
  const char* apiPath = "/channels/1417/field/1/last.txt";

  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println(F("Connection to ThingSpeak failed"));
    client.stop();
    return;
  }

  // Create the HTTP GET request
  client.print(String("GET ") + apiPath + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  // Wait for response
  unsigned long timeout = millis();
  while (client.connected() && !client.available()) {
    if (millis() - timeout > TIMEOUT) {
      Serial.println(F(">>> Client Timeout!"));
      client.stop();
      return;
    }
  }

  // Read the response
  bool headersEnd = false;
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    line.trim();

    if (!headersEnd) {
      if (line.length() == 0) {
        headersEnd = true;
      }
      continue;
    }

    if (line.length() > 0) {
      _colorName = line;
      break;
    }
  }

  // Read any remaining data
  while (client.available()) {
    client.read();
  }

  client.stop();

  _colorName.trim();
  _colorName.toLowerCase();

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
    if (_colorName.equalsIgnoreCase(color.name)) {
      _colorHex = color.color;
      break;
    }
  }

}

String CheerLights::getCurrentColor() {
  _fetchColor();
  return _colorName;
}

String CheerLights::currentColorName() {
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
