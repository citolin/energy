#include "utils.h"

int random() {
	return rand() % 100;
}

float randomFloat(unsigned maxValue) {
	return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/maxValue));
}

float addToMeasure(float average, float newValue, unsigned timesMeasured) {
	return ( average + ( (newValue - average) / timesMeasured )  );
}

void printUnorderedMap( std::unordered_map<const char*, float> m ) {
	Serial.println("\nPrinting Unordered Map");
	for( std::unordered_map<const char*,float>::iterator it = m.begin(); it != m.end(); it++ )
		Serial.printf("%s\t%f\n", it->first, it->second);
}