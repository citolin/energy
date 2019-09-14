#include <Arduino.h>

#include <WiFi.h>
// #include "ApplicationManager.h"

// ApplicationManager app;

#include "ApplicationManager.h"

#define SSID "ICTS"
#define PASSWORD "icts@2019"
#define BROKER "172.17.5.20"

ApplicationManager *app;

void setup()
{
	Serial.begin(9600);
	pinMode(2, OUTPUT);
	EEPROM.begin(SIZE_OF_EEPROM);

	delay(2000);

	Serial.println("\nStarting setup... \n");

	app = new ApplicationManager();

	Serial.println("Setup finished.");
}

void loop()
{
	app->loop();
}