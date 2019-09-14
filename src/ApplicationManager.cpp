#include "ApplicationManager.h"

ApplicationManager::ApplicationManager() : measures(QUANTITIES_MEASURED)
{
    // this->lastBroadcast = 0;
    // this->lastMeasure = 0;
    
    setBroadcastFrequency(10);
    this->broadcastFreq = (unsigned long) getBroadcastFrequency() * 1000;
    
    this->resetMeasures();

    this->ade = new ADE9000();
    this->comm = new CommunicationManager();
}

ApplicationManager::~ApplicationManager() {}

void ApplicationManager::broadcast() {
    String var = "{\"c\": " + String(this->measures[CURRENT]) \
        + ", \"t\": " + String(this->measures[VOLTAGE]) \
        + ", \"f\": " + String(this->measures[FREQUENCY]) \
        + ", \"fp\": " + String(this->measures[POWER_FACTOR]) \
        + ", \"pat\": " + String(this->measures[ACTIVE_POWER]) \
        + ", \"pr\": " + String(this->measures[REACTIVE_POWER]) \
        + ", \"pap\":" + String(this->measures[APPARENT_POWER]) + " }";

    Serial.printf("Broadcasting %s\n", WiFi.macAddress().c_str() );    
    this->comm->broadcast(var);
}

void ApplicationManager::resetMeasures() {
    for(short i=0;i<this->measures.size();i++)
        this->measures[i] = 0.0f;
    this->timesAveraged = 0;
}

void ApplicationManager::averageADE() {
    this->timesAveraged++;

    std::function<float(void)> pf[QUANTITIES_MEASURED] = {
        std::bind(&ADE9000::getCurrent, this->ade),
        std::bind(&ADE9000::getVoltage, this->ade),
        std::bind(&ADE9000::getFrequency, this->ade),
        std::bind(&ADE9000::getPowerFactor, this->ade),
        std::bind(&ADE9000::getApparentPower, this->ade),
        std::bind(&ADE9000::getActivePower, this->ade),
        std::bind(&ADE9000::getReactivePower, this->ade)
    };    

    for(uint8_t i=0;i<QUANTITIES_MEASURED;i++)
        this->measures[i] = addToMeasure( this->measures[i], pf[i](), this->timesAveraged );

    // Serial.println("\n--------------------------------");
    // Serial.printf("Times read: %u\n", this->timesAveraged);
    // Serial.printf("[CURRENT]: \t%f\n[VOLTAGE]: \t%f\n[FREQUENCY]: \t%f\n[POWER FACTOR]: \t%f\n", this->measures[CURRENT], this->measures[VOLTAGE], this->measures[FREQUENCY], this->measures[POWER_FACTOR]);
    // Serial.printf("[APARENT POWER]: \t%f\n[ACTIVE POWER]: \t%f\n[REACTIVE POWER]: \t%f\n", this->measures[APPARENT_POWER], this->measures[ACTIVE_POWER], this->measures[REACTIVE_POWER]);
}

void ApplicationManager::loop()
{
    unsigned actualMillis = millis();

    this->comm->loop();

    // Read ADE and take measure
    if( (actualMillis - this->lastMeasure) > ADE_READ_FREQUENCY )
    {
        this->lastMeasure = millis();
        this->averageADE();
    }

    // Broadcast values 
    if( (actualMillis - this->lastBroadcast) > this->broadcastFreq )
    {
        this->lastBroadcast = millis();
        this->broadcast();
    }
}