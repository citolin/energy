#include <Arduino.h>
#include <EEPROM.h>

#define SIZE_OF_EEPROM 1000

#define OFFSET_BROADCAST_FREQUENCY 1

String readStringFromEEPROM(short start, short end);


float getBroadcastFrequency();
bool setBroadcastFrequency(float frequency);