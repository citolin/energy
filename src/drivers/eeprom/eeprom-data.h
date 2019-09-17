#include <Arduino.h>
#include <EEPROM.h>
#include <utility>

#define OFFSET_BROADCAST_FREQUENCY 1

#define OFFSET_WIFI 900

namespace DATA
{
String readString(unsigned offset);
bool writeString(const char *str, uint8_t length, unsigned startOffset);

float readBroadcastFrequency();
bool writeBroadcastFrequency(float frequency);

bool writeWifi(String ssid, String password);
std::pair<String, String> readWifi();
} // namespace DATA
