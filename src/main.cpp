#include <Arduino.h>

#include "ApplicationManager.h"

ApplicationManager *app;

#define EEPROM_SIZE 4096

void setup()
{
	Serial.begin(9600);

	EEPROM.begin(EEPROM_SIZE);

	delay(2000);

	Serial.println("\nStarting setup... \n");

	app = new ApplicationManager();

	Serial.println("Setup finished.");
}

void loop()
{
	app->loop();
}