#ifndef MQTTABSTRACTION_H_
#define MQTTABSTRACTION_H_

#include <Arduino.h>
#include <MQTT.h>
#include <WiFi.h>
#include <functional>

class MQTTAbstraction
{
private:
    WiFiClient net;
    MQTTClient client;
    String broker;

    std::function<void(String)> onDataCallback;


public:
    MQTTAbstraction(const char *broker);
    ~MQTTAbstraction();

    void registerOnDataCallback(std::function<void(String)> callback);    

    void publish(char *topic, char *data);
    void parser(String &topic, String &payload);
    void connect();
    void loop();

    void start();
    void stop();
};

#endif