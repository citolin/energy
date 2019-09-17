#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

#include <WiFiClient.h>

#define HOST "192.168.0.5"
#define PORT 1200

class TCPClient {
private:
    WiFiClient socket;
    String host;
    unsigned port;

public:
    TCPClient();
    ~TCPClient();


};

#endif