#ifndef MQTTSINGLETON_H_
#define MQTTSIGLETON_H_

#include <Arduino.h>
#include "MQTTAbstraction.h"

class MQTTSingleton {
    private:
        static MQTTAbstraction *instance;
        MQTTSingleton();

    public:
        static MQTTAbstraction* getInstance(const char *broker);
        static void clearInstance();
};

#endif