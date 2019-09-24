#include "ApplicationManager.h"

#include "drivers/events.h"

ApplicationManager::ApplicationManager() : measures(QUANTITIES_MEASURED)
{
    this->lastBroadcast = millis();
    this->lastMeasure = millis();

    DATA::writeLoraID(5);

    this->mapMeasures = {
        {MAP_CURRENT, 0.0f}, {MAP_VOLTAGE, 0.0f}, {MAP_FREQUENCY, 0.0f}, {MAP_POWER_FACTOR, 0.0f}, {MAP_APPARENT_POWER, 0.0f}, {MAP_ACTIVE_POWER, 0.0f}, {MAP_REACTIVE_POWER, 0.0f}};

    this->broadcastFreq = convertSecToMilli(DATA::readBroadcastFrequency());

    this->resetMeasures();

    this->ade = new ADE9000();
    this->comm = new CommunicationManager();
    this->comm->registerOnEventCallback(std::bind(&ApplicationManager::onCallback, this, std::placeholders::_1, std::placeholders::_2));
}

ApplicationManager::~ApplicationManager() {}

void ApplicationManager::broadcast()
{
    short i = 0;
    for (auto &it : this->mapMeasures)
    {
        it.second = this->measures[i];
        i++;
    }
    this->comm->broadcast(this->mapMeasures);
}

void ApplicationManager::resetMeasures()
{
    for (short i = 0; i < this->measures.size(); i++)
        this->measures[i] = 0.0f;
    this->timesAveraged = 0;
}

void ApplicationManager::averageADE()
{
    this->timesAveraged++;

    static std::function<float(void)> pf[QUANTITIES_MEASURED] = {
        std::bind(&ADE9000::getCurrent, this->ade),
        std::bind(&ADE9000::getVoltage, this->ade),
        std::bind(&ADE9000::getFrequency, this->ade),
        std::bind(&ADE9000::getPowerFactor, this->ade),
        std::bind(&ADE9000::getApparentPower, this->ade),
        std::bind(&ADE9000::getActivePower, this->ade),
        std::bind(&ADE9000::getReactivePower, this->ade)};

    for (uint8_t i = 0; i < QUANTITIES_MEASURED; i++)
        this->measures[i] = addToMeasure(this->measures[i], pf[i](), this->timesAveraged);

    // this->printGeneral();
}

void ApplicationManager::printGeneral()
{
    Serial.println("\n--------------------------------");
    Serial.printf("Times read: %u\n", this->timesAveraged);
    Serial.printf("[CURRENT]: \t%f\n[VOLTAGE]: \t%f\n[FREQUENCY]: \t%f\n[POWER FACTOR]: \t%f\n", this->measures[CURRENT], this->measures[VOLTAGE], this->measures[FREQUENCY], this->measures[POWER_FACTOR]);
    Serial.printf("[APARENT POWER]: \t%f\n[ACTIVE POWER]: \t%f\n[REACTIVE POWER]: \t%f\n", this->measures[APPARENT_POWER], this->measures[ACTIVE_POWER], this->measures[REACTIVE_POWER]);
}

std::unordered_map<const char *, float> ApplicationManager::onCallback(uint8_t event, std::unordered_map<const char *, float> params)
{
    switch (event)
    {
    case PROTOCOL_BROADCAST:
    {
        Serial.println("-- EVENT READ MEASURES");
        this->broadcast();
        return this->mapMeasures;
        break;
    }
    case PROTOCOL_READ_WRITE_BROADCAST_FREQ:
    {
        Serial.println("-- EVENT WRITE BROADCAST FREQUENCY");

        DATA::writeBroadcastFrequency(params["frequency"]);
        this->broadcastFreq = convertSecToMilli(params["frequency"]);
        return std::unordered_map<const char *, float>{{"status", 1.0f}};
        break;
    }
    case PROTOCOL_PRINT_STATUS:
    {
        Serial.println("-- EVENT PRINT GENERAL");
        this->printGeneral();
        return std::unordered_map<const char *, float>{{"status", 1.0f}};
    }
    default:
    {
        return std::unordered_map<const char *, float>{{"status", 0.0f}};
    }
    }
    return std::unordered_map<const char *, float>{{"status", 0.0f}};
}

// {G10}

void ApplicationManager::loop()
{
    unsigned actualMillis = millis();

    this->comm->loop();

    // Read ADE and take measure
    if ((actualMillis - this->lastMeasure) > ADE_READ_FREQUENCY)
    {
        this->lastMeasure = actualMillis;
        this->averageADE();
    }

    // Broadcast values
    if ((actualMillis - this->lastBroadcast) > this->broadcastFreq)
    {
        this->lastBroadcast = actualMillis;
        this->broadcast();
    }
}