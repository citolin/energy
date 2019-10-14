#ifndef MQTTABSTRACTION_H_
#define MQTTABSTRACTION_H_

#include <Arduino.h>
#include <MQTT.h>
#include <WiFi.h>

#include "../../abstraction/CallbackClass.h"

#define CONFIG_TOPIC "/config"
#define RETURN_TOPIC "/return"

class MQTTAbstraction : public CallbackClass
{
private:
    WiFiClient net;
    MQTTClient client;
    String broker;

    void begin();

public:
    MQTTAbstraction(const char *broker);
    ~MQTTAbstraction();

    void publish(const char *topic, const char *data);
    void publish(const char *topic, const char *data, int length);
    
    void publishConfig(String data);
    void parser(String &topic, String &payload);
    void connect();
    void loop();

    void start();
    void stop();
};

#endif