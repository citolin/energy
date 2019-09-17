#include "SerialAbstraction.h"

SerialAbstraction::SerialAbstraction() {}

SerialAbstraction::~SerialAbstraction() {}

void SerialAbstraction::loop()
{
    if (Serial.available())
    {
        String temp = Serial.readStringUntil('\n');

        if (temp.length())
            this->callCallback(temp);
    }
}