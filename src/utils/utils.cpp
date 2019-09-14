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