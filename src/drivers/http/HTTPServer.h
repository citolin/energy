#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

// https://github.com/me-no-dev/ESPAsyncWebServer

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <functional>

#define SERVER_PORT 80


class HTTPServer {
private:
    AsyncWebServer server;

    std::function<void(String)> onDataCallback;
    void onConfig();

public:
    HTTPServer();
    ~HTTPServer();

    void registerOnDataCallback( std::function<void(String)> callback );

    void start();
    void stop();
    void loop();
};

#endif
