#include "eeprom-data.h"

String readStringFromEEPROM(short start, short end)
{
    char data[ (end - start) + 2];
    memset(data, '\0', sizeof(data));
    short index = 0;

    for(short i = start; i < end; i++)
    {
        data[index] = (char)EEPROM.read(i);
        index++;
    } 

    return String(data);
}

bool setBroadcastFrequency(float frequency) {
    EEPROM.writeFloat(OFFSET_BROADCAST_FREQUENCY, frequency);
    return EEPROM.commit();
}

float getBroadcastFrequency()
{
    return EEPROM.readFloat(OFFSET_BROADCAST_FREQUENCY);
}