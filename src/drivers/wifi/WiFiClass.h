#ifndef WIFICLASS_H_
#define WIFICLASS_H_

#include "WiFi.h"

class WiFiClass
{
private:
    void onEvent();

    const char *ssid;
    const char *password;

public:
    WiFiClass();
    ~WiFiClass();

    void connectToWifi(const char *ssid, const char *password);
}

#endif