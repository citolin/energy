#include "CallbackClass.h"

CallbackClass::CallbackClass()
{
    this->callback = nullptr;
    this->secondCallback = nullptr;
}

CallbackClass::~CallbackClass() {}

void CallbackClass::registerCallback(std::function<void(String)> callback)
{
    this->callback = callback;
}

void CallbackClass::registerCallback(std::function<void(String, String)> callback) {
    this->secondCallback = callback;
}

void CallbackClass::callCallback(String data)
{
    if (this->callback)
        this->callback(data);
}

void CallbackClass::callCallback(String data, String secondData)
{
    if (this->secondCallback)
        this->secondCallback(data, secondData);
}