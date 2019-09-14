#ifndef COMMUNICATIONMANAGER_H_
#define COMMUNICATIONMANAGER_H_

#include <WiFi.h>
#include "mqtt/MQTTSingleton.h"
#include "http/HTTPServer.h"
#include "serial/SerialAbstraction.h"
#include "wifi/wifi-utils.h"
#include "udp/UDPAbstraction.h"

#define SSID "ICTS"
#define PASSWORD "icts@2019"
#define BROKER "172.17.5.20"

class CommunicationManager
{
private:
    MQTTAbstraction *mqtt;
    SerialAbstraction *serial;
    HTTPServer *server;
    UDPAbstraction *udp;

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
    void onUDPCallback(String data);
};

#endif