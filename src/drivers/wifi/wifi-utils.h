#include <Arduino.h>
#include <WiFi.h>

namespace WIFI{
void connectToWifiSynch(const char *ssid, const char *password);
void connectToWifiAssynch(const char *ssid, const char *password);
void reconnectToWifi();
void WiFiEvent(WiFiEvent_t event);
}
