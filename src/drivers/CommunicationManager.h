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
#include "lora/LORA.h"

#define READ_LENGTH    0        // Because the ID get's removed
#define MINIMAL_PROTOCOL_SIZE 3
#define INDEX_HEADER 0
#define PROTOCOL_ID 0
#define PROTOCOL_LAST_LETTER 2

// {OICTS;icts@icts}

#define INDEX_WIFI          0
#define INDEX_PASSWORD      1

#define MQTT_GENERAL_TOPIC "/devices"
// #define BROKER "192.168.0.7"
// #define BROKER "35.247.213.52"
// #define BROKER "172.17.1.200"
#define BROKER "172.17.1.179"

class CommunicationManager
{
private:
    MQTTAbstraction *mqtt;
    SerialAbstraction *serial;
    HTTPServer *server;
    UDPAbstraction *udp;
    LORA *lora;

    bool isWifiConnected;

    std::function<std::unordered_map<const char *, float>(uint8_t event, std::unordered_map<const char *, float> params)> callback;

    void onWiFiEvent(WiFiEvent_t event);
    void startWifiCommunications();
    void stopWifiCommunications();
    
    String preParser(String data);
    String parser(String data);
    String formatProtocol(char id, String payload);
    String formatACK(char id);
    String formatMeasuresBroadcast(std::unordered_map<const char*,float> values);
    bool formatBinaryMeasuresBroadcast(std::unordered_map<const char*, float> values, char *buff, size_t length);

public:
    CommunicationManager();
    ~CommunicationManager();

    void loop();
    void broadcast(std::unordered_map<const char*,float> values);

    // Callbacks
    void onSerialCallback(String data);
    void onLORACallback(String data);
    void onMQTTCallback(String data);
    void onHTTPCallback(String data);
    void onUDPCallback(String data, String senderIP);

    void registerOnEventCallback(std::function<std::unordered_map<const char*,float>(uint8_t event, std::unordered_map<const char *, float> params)> callback);
};

#endif