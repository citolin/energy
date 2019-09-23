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

void UDPAbstraction::loop() {
    if(!WiFi.isConnected())
        return;
        
    int packetSize = udpListener.parsePacket();
    if(packetSize)
    {
        String data = udpListener.readStringUntil('\n');
        IPAddress senderIP = udpListener.remoteIP();

        this->callCallback(data, senderIP.toString());
    }
}

void UDPAbstraction::broadcast(String data) {
    if(!WiFi.isConnected())
        return;

    int packet = udpBroadcast.beginPacket(UDP_BROADCAST_ADDRESS, 9998);
    size_t size = udpBroadcast.write( (const uint8_t*) data.c_str(), (size_t) data.length());
    bool sent = udpBroadcast.endPacket();

    // Serial.printf("[UDP Broadcast] Packets: %d - %d bytes -- Sent: %s\n", packet, size, sent?"true":"false");
}