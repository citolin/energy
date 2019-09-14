#include "UDPAbstraction.h"

UDPAbstraction::UDPAbstraction() {
}

UDPAbstraction::~UDPAbstraction() { }

void UDPAbstraction::start() {
    udpListener.begin(UDP_LISTENER_PORT);
    udpBroadcast.begin(UDP_BROADCAST_PORT);
}

void UDPAbstraction::stop() {
    udpListener.stop();
    udpBroadcast.stop();
}

void UDPAbstraction::registerOnDataCallback( std::function<void(String)> callback ) {
    this->onDataCallback = callback;
}

void UDPAbstraction::loop() {
    if(!WiFi.isConnected())
        return;
        
    int packetSize = udpListener.parsePacket();
    if(packetSize)
    {
        String data = udpListener.readStringUntil('\n');
        
        if(onDataCallback)
            onDataCallback(data);
    }
}

void UDPAbstraction::broadcast(String data) {
    int packet = udpBroadcast.beginPacket(UDP_BROADCAST_ADDRESS, UDP_BROADCAST_PORT);
    size_t size = udpBroadcast.write( (const uint8_t*) data.c_str(), (size_t) data.length());
    bool sent = udpBroadcast.endPacket();

    Serial.printf("[UDP Broadcast]\n\tPacket: %d\n\tSize: %d\n\tSent: %s\n", packet, size, sent?"true":"false");
}