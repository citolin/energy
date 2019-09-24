#include "LORA.h"

LORA::LORA() {
    Serial2.begin(LORA_BAUDRATE);
}

LORA::~LORA() {}

void LORA::loop()
{
    if (Serial2.available())
    {
        String temp = Serial2.readStringUntil('\n');
        Serial.printf("[LORA]: %s\n", temp.c_str() );
        if (temp.length())
            this->callCallback(temp);
    }
}

void LORA::write(String data) {
    Serial2.println(data);
}

void LORA::write(char *data) {
    Serial2.println(data);
}