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

unsigned convertSecToMilli(float val) {
	return (unsigned)(val * 1000);
}

void printUnorderedMap( std::unordered_map<const char*, float> m ) {
	Serial.println("\nPrinting Unordered Map");
	for( std::unordered_map<const char*,float>::iterator it = m.begin(); it != m.end(); it++ )
		Serial.printf("%s\t%f\n", it->first, it->second);
}

void printStringVector( std::vector<String> v ) {
	Serial.println("\n Printing vector");
	for(short i=0;i<v.size(); i++)
		Serial.printf("[%d]\t%s\n", i, v[i].c_str());
}

std::vector<String> splitString(String data, char delimiter) {
	Serial.printf("Split string\n\t%s\nDelimiter: %c\n", data.c_str(), delimiter);
	std::vector<String> splits;
	unsigned start = 0;
	unsigned i = 0;
	// It starts at 2 and ends at 4
	// String sub = data.substring(2, 5);
	
	for(i=0;i<data.length();i++)
	{
		if( data.charAt(i) == delimiter )
		{
			splits.push_back( data.substring(start, i) );
			start = i+1;
		}
	}

	if( (start != 0) && (start != i) )
		splits.push_back( data.substring(start, i) );

	printStringVector(splits);
	return splits;
}
