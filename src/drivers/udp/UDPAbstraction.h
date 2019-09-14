#ifndef UDPABSTRACTION_H_
#define UDPABSTRACTION_H_

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <functional>

#define UDP_LISTENER_PORT 9997
#define UDP_BROADCAST_PORT 9998
#define UDP_BROADCAST_ADDRESS "255.255.255.255"

class UDPAbstraction {
    private:
        WiFiUDP udpListener;
        WiFiUDP udpBroadcast;

        std::function<void(String)> onDataCallback;

    public:
        UDPAbstraction();
        ~UDPAbstraction();

        void registerOnDataCallback( std::function<void(String)> callback );

        void start();
        void stop();

        void broadcast(String data);
        void loop();
};

#endif