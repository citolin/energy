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

#define MINIMAL_PROTOCOL_SIZE 3
#define INDEX_HEADER 0
#define PROTOCOL_ID 0
#define PROTOCOL_HEADER '{'
#define PROTOCOL_END '}'
#define ACK 'A'
// Protocol header index is:    protocol.length() - 2
//                    {O}\n             4         - 2 =     2
#define PROTOCOL_LAST_LETTER 2

#define ERROR_PROTOCOL_UNFORMATED   "{XUNFORMATED PROTOCOL}"
#define ERROR_PROTOCOL_SMALL        "{XPROTOCOL TOO SMALL}"
#define ERROR_UNKOWN_PROTOCOL       "{XUNKOWN PROTOCOL}"

#define INDEX_WIFI          0
#define INDEX_PASSWORD      1

#define MQTT_GENERAL_TOPIC "/devices"
#define BROKER "172.17.5.81"

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