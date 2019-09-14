#include "ApplicationManager.h"

ApplicationManager::ApplicationManager() : measures(QUANTITIES_MEASURED)
{
    setBroadcastFrequency(10);
    this->broadcastFreq = (unsigned long) getBroadcastFrequency() * 1000;
    pinMode(2, OUTPUT);

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

    this->measures[CURRENT] = addToMeasure( this->measures[CURRENT], this->ade->getCurrent(), this->timesAveraged );
    this->measures[VOLTAGE] = addToMeasure( this->measures[VOLTAGE], this->ade->getVoltage(), this->timesAveraged );
    this->measures[FREQUENCY] = addToMeasure( this->measures[FREQUENCY], this->ade->getFrequency(), this->timesAveraged );
    this->measures[POWER_FACTOR] = addToMeasure( this->measures[POWER_FACTOR], this->ade->getPowerFactor(), this->timesAveraged );
    this->measures[APPARENT_POWER] = addToMeasure( this->measures[APPARENT_POWER], this->ade->getApparentPower(), this->timesAveraged );
    this->measures[ACTIVE_POWER] = addToMeasure( this->measures[ACTIVE_POWER], this->ade->getActivePower(), this->timesAveraged );
    this->measures[REACTIVE_POWER] = addToMeasure( this->measures[REACTIVE_POWER], this->ade->getReactivePower(), this->timesAveraged );

    // Serial.println("\n--------------------------------");
    // Serial.printf("Times read: %u\n", this->timesAveraged);
    // Serial.printf("[CURRENT]: \t%f\n[VOLTAGE]: \t%f\n[FREQUENCY]: \t%f\n[POWER FACTOR]: \t%f\n", this->measures[CURRENT], this->measures[VOLTAGE], this->measures[FREQUENCY], this->measures[POWER_FACTOR]);
    // Serial.printf("[APARENT POWER]: \t%f\n[ACTIVE POWER]: \t%f\n[REACTIVE POWER]: \t%f\n", this->measures[APPARENT_POWER], this->measures[ACTIVE_POWER], this->measures[REACTIVE_POWER], this->measures[POWER_FACTOR]);
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