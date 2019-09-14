#include "CommunicationManager.h"

CommunicationManager::CommunicationManager()
{
    mqtt = MQTTSingleton::getInstance(BROKER);
    mqtt->connectAssync();
    mqtt->registerOnDataCallback( std::bind(&CommunicationManager::onMQTTCallback, this, std::placeholders::_1) );

    Serial.println("> Started MQTT Client");


    serial = new SerialAbstraction();
    serial->registerOnDataCallback( std::bind(&CommunicationManager::onSerialCallback, this, std::placeholders::_1) );

    Serial.println("> Started Serial Reader");


    server = new HTTPServer();
    server->registerOnDataCallback( std::bind(&CommunicationManager::onHTTPCallback, this, std::placeholders::_1) );

    Serial.println("> Started HTTP Server");

    udp = new UDPAbstraction();
    udp->registerOnDataCallback( std::bind(&CommunicationManager::onUDPCallback, this, std::placeholders::_1) );

    WiFi.onEvent( std::bind( &CommunicationManager::onWiFiEvent, this, std::placeholders::_1 ) );
    connectToWifiAssynch(SSID, PASSWORD);
}

CommunicationManager::~CommunicationManager() {}

void CommunicationManager::onSerialCallback(String data) {
    Serial.printf("-> [SERIAL]: %s\n", data.c_str());


}

void CommunicationManager::onMQTTCallback(String data) {
    Serial.printf(">>> [MQTT]: %s\n", data.c_str());


}

void CommunicationManager::onHTTPCallback(String data) {
    Serial.printf(">>> [HTTP]: %s\n", data.c_str());


}

void CommunicationManager::onUDPCallback(String data) {
    Serial.printf(">>> [UDP]: %s\n", data.c_str());


}

void CommunicationManager::broadcast(String data)
{
    mqtt->publish((char *)WiFi.macAddress().c_str(), (char *)data.c_str());
    mqtt->publish("/devices", (char *)WiFi.macAddress().c_str());
}

void CommunicationManager::loop() {
    reconnectToWifi();

    mqtt->loop();
    serial->loop();
    udp->loop();
}



void CommunicationManager::startWifiCommunications() {
    udp->start();
    server->start();
}

void CommunicationManager::stopWifiCommunications() {
    udp->stop();
    server->stop();
}

void CommunicationManager::onWiFiEvent(WiFiEvent_t event) {
    Serial.print("> ");
    
    switch (event) {
        case SYSTEM_EVENT_WIFI_READY: 
            Serial.println("WiFi interface ready");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case SYSTEM_EVENT_STA_START:
            Serial.println("WiFi client started");
            break;
        case SYSTEM_EVENT_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("Disconnected from WiFi access point");
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.print("Obtained IP address: ");
            Serial.println(WiFi.localIP());
            startWifiCommunications();
            break;
        case SYSTEM_EVENT_STA_LOST_IP:
            Serial.println("Lost IP address and IP address is reset to 0");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
            Serial.println("WiFi Protected Setup (WPS): succeeded in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_FAILED:
            Serial.println("WiFi Protected Setup (WPS): failed in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
            Serial.println("WiFi Protected Setup (WPS): timeout in enrollee mode");
            break;
        case SYSTEM_EVENT_STA_WPS_ER_PIN:
            Serial.println("WiFi Protected Setup (WPS): pin code in enrollee mode");
            break;
        case SYSTEM_EVENT_AP_START:
            Serial.println("WiFi access point started");
            break;
        case SYSTEM_EVENT_AP_STOP:
            Serial.println("WiFi access point  stopped");
            break;
        case SYSTEM_EVENT_AP_STACONNECTED:
            Serial.println("Client connected");
            break;
        case SYSTEM_EVENT_AP_STADISCONNECTED:
            Serial.println("Client disconnected");
            break;
        case SYSTEM_EVENT_AP_STAIPASSIGNED:
            Serial.println("Assigned IP address to client");
            break;
        case SYSTEM_EVENT_AP_PROBEREQRECVED:
            Serial.println("Received probe request");
            break;
        case SYSTEM_EVENT_GOT_IP6:
            Serial.println("IPv6 is preferred");
            break;
        case SYSTEM_EVENT_ETH_START:
            Serial.println("Ethernet started");
            break;
        case SYSTEM_EVENT_ETH_STOP:
            Serial.println("Ethernet stopped");
            break;
        case SYSTEM_EVENT_ETH_CONNECTED:
            Serial.println("Ethernet connected");
            break;
        case SYSTEM_EVENT_ETH_DISCONNECTED:
            Serial.println("Ethernet disconnected");
            break;
        case SYSTEM_EVENT_ETH_GOT_IP:
            Serial.println("Obtained IP address");
            break;
    }
}