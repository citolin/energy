#ifndef UDPABSTRACTION_H_
#define UDPABSTRACTION_H_

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#include "../../abstraction/CallbackClass.h"

#define UDP_LISTENER_PORT 9998
#define UDP_BROADCAST_PORT 9997
#define UDP_BROADCAST_ADDRESS "172.17.255.255"
// #define UDP_BROADCAST_ADDRESS "192.168.0.255"

class UDPAbstraction : public CallbackClass
{
    private:
        WiFiUDP udpListener;
        WiFiUDP udpBroadcast;

    public:
        UDPAbstraction();
        ~UDPAbstraction();

        void start();
        void stop();

        void broadcast(String data);
        void loop();
};

#endif