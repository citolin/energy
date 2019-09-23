#ifndef APPLICATIONMANAGER_H_
#define APPLICATIONMANAGER_H_

#include <Arduino.h>
#include <unordered_map>
#include <functional>
#include "drivers/CommunicationManager.h"
#include "drivers/ade/ade9000.h"

#include "drivers/eeprom/eeprom-data.h"
#include "utils/utils.h"

#define ADE_READ_FREQUENCY 3000

#define CURRENT 0
#define VOLTAGE 1
#define FREQUENCY 2
#define POWER_FACTOR 3
#define ACTIVE_POWER 4
#define REACTIVE_POWER 5
#define APPARENT_POWER 6

#define QUANTITIES_MEASURED 7

class ApplicationManager
{
private:
    // Components
    CommunicationManager *comm;
    ADE9000 *ade;

    // Time variables
    unsigned long lastBroadcast;
    unsigned long lastMeasure;
    unsigned long broadcastFreq;

    // Measured quantities
    std::vector<float> measures;
    unsigned timesAveraged;
    std::unordered_map<const char *, float> mapMeasures;

    void broadcast();
    void averageADE();
    void printGeneral();

public:
    ApplicationManager();
    ~ApplicationManager();

    void loop();
    void resetMeasures();
    std::unordered_map<const char *, float> onCallback(uint8_t event, std::unordered_map<const char *, float> params);
};

#endif