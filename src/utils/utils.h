#include <Arduino.h>
#include <unordered_map>
#include <vector>

int random();
float randomFloat(unsigned maxValue);
float addToMeasure(float average, float newValue, unsigned timesMeasured);

unsigned convertSecToMilli(float val);

void printUnorderedMap( std::unordered_map<const char*, float> m );
void printStringVector( std::vector<String> v );

std::vector<String> splitString(String data, char delimiter);