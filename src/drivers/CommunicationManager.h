#ifndef COMMUNICATIONMANAGER_H_
#define COMMUNICATIONMANAGER_H_

#include <Arduino.h>
#include <WiFi.h>
#include <unordered_map>
#include <functional>
#include "mqtt/MQTTSingleton.h"
#include "http/HTTPServer.h"
#include "serial/SerialAbstraction.h"
#include "wifi/wifi-utils.h"
#include "udp/UDPAbstraction.h"

// #define SSID "ICTS"
// #define PASSWORD "icts@2019"
// #define BROKER "172.17.5.20"

#define SSID "citolin"
#define PASSWORD "jirafo@1966"
#define BROKER "192.168.0.5"

class CommunicationManager
{
private:
    MQTTAbstraction *mqtt;
    SerialAbstraction *serial;
    HTTPServer *server;
    UDPAbstraction *udp;

    bool isWifiConnected;

    std::function<std::unordered_map<const char *, float>(uint8_t event, std::unordered_map<const char *, float> params)> callback;

    void onWiFiEvent(WiFiEvent_t event);
    void startWifiCommunications();
    void stopWifiCommunications();

public:
    CommunicationManager();
    ~CommunicationManager();

    void loop();
    void broadcast(String data);

    // Callbacks
    void onSerialCallback(String data);
    void onMQTTCallback(String data);
    void onHTTPCallback(String data);
    void onUDPCallback(String data, String senderIP);

    void registerOnEventCallback(std::function<std::unordered_map<const char*,float>(uint8_t event, std::unordered_map<const char *, float> params)> callback);
};

#endif