#include "CommunicationManager.h"

#include "events.h"
#include "../utils/utils.h"
#include "eeprom/eeprom-data.h"

CommunicationManager::CommunicationManager()
{
    isWifiConnected = false;
    this->callback = nullptr;

    // 1. Connect to Wifi
    WiFi.onEvent(std::bind(&CommunicationManager::onWiFiEvent, this, std::placeholders::_1));
    std::pair<String, String> wifiData = DATA::readWifi();
    WIFI::connectToWifiAssynch(wifiData.first.c_str(), wifiData.second.c_str());

    // 2. Connect to MQTT Broker
    mqtt = MQTTSingleton::getInstance(BROKER);
    mqtt->registerCallback(static_cast<std::function<void(String)>>(std::bind(&CommunicationManager::onMQTTCallback, this, std::placeholders::_1)));

    Serial.println("> Started MQTT Client");

    // 3. Start reading Serial
    serial = new SerialAbstraction();
    serial->registerCallback(static_cast<std::function<void(String)>>(std::bind(&CommunicationManager::onSerialCallback, this, std::placeholders::_1)));

    Serial.println("> Started Serial Reader");

    // 4. Start HTTP API
    server = new HTTPServer();
    server->registerCallback(static_cast<std::function<void(String)>>(std::bind(&CommunicationManager::onHTTPCallback, this, std::placeholders::_1)));

    Serial.println("> Started HTTP Server");

    // 5. Start UDP Socket
    udp = new UDPAbstraction();
    udp->registerCallback(static_cast<std::function<void(String, String)>>(std::bind(&CommunicationManager::onUDPCallback, this, std::placeholders::_1, std::placeholders::_2)));
}

CommunicationManager::~CommunicationManager() {}

void CommunicationManager::registerOnEventCallback(std::function<std::unordered_map<const char *, float>(uint8_t event, std::unordered_map<const char *, float> params)> callback)
{
    this->callback = callback;
}

void CommunicationManager::onSerialCallback(String data)
{
    Serial.printf(">>> [SERIAL]: %s\n", data.c_str());

    String re = preParser(data);
    Serial.println(re);
}

void CommunicationManager::onMQTTCallback(String data)
{
    Serial.printf(">>> [MQTT]: %s\n", data.c_str());

    String re = preParser(data);
    mqtt->publishConfig(re);
}

void CommunicationManager::onHTTPCallback(String data)
{
    Serial.printf(">>> [HTTP]: %s\n", data.c_str());
}

void CommunicationManager::onUDPCallback(String data, String senderIP)
{
    Serial.printf(">>> [UDP]: %s - %s\n", senderIP.c_str(), data.c_str());
}

void CommunicationManager::broadcast(String data)
{
    if (!isWifiConnected)
        Serial.println("> Couldn't broadcast due to no wifi connection.");

    mqtt->publish((char *)WiFi.macAddress().c_str(), (char *)data.c_str());
    mqtt->publish(MQTT_GENERAL_TOPIC, (char *)WiFi.macAddress().c_str());

    udp->broadcast((char *)WiFi.macAddress().c_str());
}

void CommunicationManager::loop()
{
    serial->loop();

    if (!isWifiConnected)
        return;

    mqtt->loop();
    udp->loop();
}

void CommunicationManager::startWifiCommunications()
{
    udp->start();
    server->start();
    mqtt->start();

    isWifiConnected = true;
}

void CommunicationManager::stopWifiCommunications()
{
    udp->stop();
    server->stop();
    mqtt->stop();

    isWifiConnected = false;
}

String CommunicationManager::formatProtocol(char id, String payload) {
    return String(PROTOCOL_HEADER) + String(id) + payload + String(PROTOCOL_END);
}

String CommunicationManager::formatACK(char id) {
    return String(PROTOCOL_HEADER) + String(ACK) + String(id) + String(PROTOCOL_END);
}

String CommunicationManager::preParser(String data) {
    std::vector<String> v;
    bool reading = false;
    short start = 0;
    for(short i=0;i<data.length();i++)
    {
        if(data[i] == PROTOCOL_HEADER && !reading)
        {
            reading = true;
            start = i + 1;
        }
        else if(data[i] == PROTOCOL_END && reading)
        {
            v.push_back( data.substring(start, i) );
            reading = false;
        }
    }

    String r = "";
    // Could do this N other ways.
    //      by having a class queue (would have to be a std::pair<uint8_t, String>)
    //      by returning the vector (more repetitive code)
    //          chose this way for no reason. I don't even like it.
    for(short i=0;i<v.size();i++)
        r += this->parser(v[i]);
    return r;
}

String CommunicationManager::parser(String data)
{
    std::unordered_map<const char *, float> params;
    std::unordered_map<const char *, float> values;

    char id =  data.charAt(PROTOCOL_ID);
    data.remove(PROTOCOL_ID, 1);
    switch (id)
    {
    // READ: {ID}       WRITE: { ID SSID ; PASSWORD } 
    case PROTOCOL_READ_WRITE_WIFI:
    {
        Serial.printf("Protocol %c - Setup WiFi\n\t%s\n", id, data.c_str());
        
        // If it's the READ wifi, then it comes with only the ID
        if(data.length() == READ_LENGTH)
        {
            std::pair<String,String> wifiSetup = DATA::readWifi();
            return this->formatProtocol(id, (wifiSetup.first + ";" + wifiSetup.second) );
        }

        // Else it's WRITE wifi
        // Parse
        std::vector<String> wifiSetup = splitString(data, ';');
        // Validate
        if ((wifiSetup.size() != 2) || (wifiSetup[INDEX_WIFI].length() == 0) || (wifiSetup[INDEX_PASSWORD].length() == 0))
            return ERROR_PROTOCOL_UNFORMATED;
        // Execute
        DATA::writeWifi(wifiSetup[INDEX_WIFI].c_str(), wifiSetup[INDEX_PASSWORD].c_str());
        return this->formatACK(id);
    }
    // READ: {ID}            WRITE: { ID BROADCAST_FREQUENCY }
    case PROTOCOL_READ_WRITE_BROADCAST_FREQ: {
        Serial.printf("Protocol F - Setup broadcast frequency\n\t%s\n", data.c_str());
        // Read frequency protocol
        if(data.length() == READ_LENGTH)
            return this->formatProtocol( id, String(DATA::readBroadcastFrequency()) );
        
        // Write frequency protocol
        float freq = data.toFloat();
        params[MAP_BROADCAST_FREQUENCY] = freq;
        values = this->callback(id, params);
        String s = this->formatACK(id);
        return s;
    }
    default:
    {
        return ERROR_UNKOWN_PROTOCOL;
    }
    }
    
    return ERROR_UNKOWN_PROTOCOL;    
}















void CommunicationManager::onWiFiEvent(WiFiEvent_t event)
{
    Serial.print("> ");

    switch (event)
    {
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
        Serial.printf("[HOST]: %s\n", WiFi.gatewayIP().toString().c_str());
        Serial.printf("Netmask: %s \n", WiFi.subnetMask().toString().c_str());
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