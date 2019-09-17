#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

// https://github.com/me-no-dev/ESPAsyncWebServer

#include <Arduino.h>
#include <ESPAsyncWebServer.h>

#include "../../abstraction/CallbackClass.h"

#define SERVER_PORT 80

class HTTPServer : public CallbackClass
{
private:
    AsyncWebServer server;

    void onConfig();

public:
    HTTPServer();
    ~HTTPServer();

    void start();
    void stop();
    void loop();
};

#endif
