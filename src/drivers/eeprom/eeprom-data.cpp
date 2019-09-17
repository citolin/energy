#include "eeprom-data.h"

bool DATA::writeBroadcastFrequency(float frequency)
{
    EEPROM.writeFloat(OFFSET_BROADCAST_FREQUENCY, frequency);
    return EEPROM.commit();
}

float DATA::readBroadcastFrequency()
{
    return EEPROM.readFloat(OFFSET_BROADCAST_FREQUENCY);
}

std::pair<String, String> DATA::readWifi()
{
    String ssid = DATA::readString(OFFSET_WIFI);
    String password = DATA::readString(OFFSET_WIFI + ssid.length() + 1);
    return std::pair<String, String>{ssid, password};
}

bool DATA::writeWifi(String ssid, String password)
{
    DATA::writeString(ssid.c_str(), ssid.length(), OFFSET_WIFI);
    DATA::writeString(password.c_str(), password.length(), OFFSET_WIFI + ssid.length() + 1);
}

bool DATA::writeString(const char *str, uint8_t length, unsigned startOffset)
{
    EEPROM.writeByte(startOffset, length);
    unsigned offset = startOffset + 1;
    for (uint8_t i = 0; i < length; i++)
    {
        EEPROM.writeChar(offset, str[i]);
        offset++;
    }
    return EEPROM.commit();
}

String DATA::readString(unsigned offset)
{
    uint8_t stringSize = EEPROM.readByte(offset);

    char data[stringSize + 1];
    memset(data, '\0', sizeof(data));
    unsigned index = 0;

    for (unsigned i = (offset + 1); i <= (offset + stringSize); i++)
    {
        data[index] = (char)EEPROM.read(i);
        index++;
    }

    return String(data);
}