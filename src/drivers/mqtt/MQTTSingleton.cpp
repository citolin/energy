#include "MQTTSingleton.h"

MQTTAbstraction* MQTTSingleton::instance = nullptr;

MQTTAbstraction* MQTTSingleton::getInstance(const char *broker)
{
    if(instance == 0)
        instance = new MQTTAbstraction(broker);

    return instance;
}

void MQTTSingleton::clearInstance()
{
    if(instance)
    {
        delete instance;
        instance = nullptr;
    }
}

MQTTSingleton::MQTTSingleton() { }
