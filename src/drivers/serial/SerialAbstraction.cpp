#include "SerialAbstraction.h"

SerialAbstraction::SerialAbstraction() {
    this->onDataCallback = nullptr;
}

SerialAbstraction::~SerialAbstraction() { }

void  SerialAbstraction::registerOnDataCallback(std::function<void(String)> callback) {
    this->onDataCallback = callback;
}

void SerialAbstraction::loop() {
    String temp = Serial.readStringUntil('\n');
    
    if(temp.length() && this->onDataCallback)
        this->onDataCallback(temp);

    // size_t size = Serial.readBytes(this->buffer, sizeof(this->buffer));    
    // if(size) {
        // Serial.printf("[READ]: %s\n", this->buffer);
        // memset(this->buffer, '\0', sizeof(this->buffer));
    // }

}