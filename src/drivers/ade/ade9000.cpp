#include "ade9000.h"
#include "../../utils/utils.h"

ADE9000::ADE9000() { }

ADE9000::~ADE9000() { }

float ADE9000::getCurrent() { return randomFloat(100); }

float ADE9000::getVoltage() { return randomFloat(250); }

float ADE9000::getFrequency() { return randomFloat(70); }

float ADE9000::getPowerFactor() { return randomFloat(300); }

float ADE9000::getApparentPower() { return randomFloat(300); }

float ADE9000::getActivePower() { return randomFloat(600); }

float ADE9000::getReactivePower() { return randomFloat(1000); }

