#include "MQTTAbstraction.h"
#include <functional>

#include "MQTTSingleton.h"

void onMessage(String &topic, String &payload) {
	MQTTAbstraction *abs = MQTTSingleton::getInstance("");
	abs->parser(topic, payload);
}

MQTTAbstraction::MQTTAbstraction(const char *broker) {
	this->broker = String(broker);

    this->client.begin(this->broker.c_str(), this->net);

	// - Shameful what I did here.
	// Had to create a singleton because the onMessage() function receives a callback of type MQTTCallbackSimple
	//		which I could not cast as a lambda function or by using std::bind
    this->client.onMessage( *onMessage );
}

MQTTAbstraction::~MQTTAbstraction() { }

void MQTTAbstraction::registerOnDataCallback(std::function<void(String)> callback) {
	this->onDataCallback = callback;
}   

void MQTTAbstraction::connectAssync() {
	if(!WiFi.isConnected())
		return;

	if( !this->client.connected()) {
		// Serial.println("Trying to connect to MQTT broker");

		if(this->client.connect(WiFi.macAddress().c_str())) {
			Serial.printf("> MQTT Broker connected at %s\n", this->broker.c_str());
			this->client.subscribe( WiFi.macAddress() + "/config" );
		}
	}
}

void MQTTAbstraction::connectSync() {
	// TODO - This creates a direct dependency from the WiFi library of the ESP32, 
	//		thus making it not general to all ARDUINO platforms
	// Serial.printf("MQTT CONNECT %d\n", this->net.connected());
	// if(!this->net.connected())
	if(!WiFi.isConnected())
		return;

	Serial.println("> Trying to connect to BROKER");
	while(!this->client.connect( WiFi.macAddress().c_str() )) {
		Serial.print(".");
		delay(1000);
	}

	Serial.println("> Broker connected");
}

void MQTTAbstraction::parser(String &topic, String &payload) {
	if(this->onDataCallback)
		this->onDataCallback(payload);
}

void MQTTAbstraction::publish(char *topic, char *data) {
	this->client.publish( (const char*) topic, (const char*) data);
}

void MQTTAbstraction::loop() {
    this->client.loop();

	if(!this->client.connected())
		this->connectAssync();
}