#include <Arduino.h>
#include <unordered_map>

int random();
float randomFloat(unsigned maxValue);
float addToMeasure(float average, float newValue, unsigned timesMeasured);
void printUnorderedMap( std::unordered_map<const char*, float> m );