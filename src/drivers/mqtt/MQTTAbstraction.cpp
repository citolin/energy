#include "MQTTAbstraction.h"
#include <functional>

#include "MQTTSingleton.h"
#include "../events.h"

void onMessage(String &topic, String &payload) {
	MQTTAbstraction *abs = MQTTSingleton::getInstance("");
	abs->parser(topic, payload);
}


// TODO - This MQTT library has a sync connection event making the timeout 0 (this case it just doesn't connect)
//      see what happens when it can't connect to the broker multiple times
// What can happen is: for it being sync, it can block other communications causing buffer size to increase and core to panic
MQTTAbstraction::MQTTAbstraction(const char *broker) {
	this->broker = String(broker);

	this->client.setOptions(10, true, 1000);
    this->client.begin(this->broker.c_str(), this->net);
	// - Shameful what I did here.
	// Had to create a singleton because the onMessage() function receives a callback of type MQTTCallbackSimple
	//		which I could not cast as a lambda function or by using std::bind
    this->client.onMessage( *onMessage );
}

MQTTAbstraction::~MQTTAbstraction() { }

void MQTTAbstraction::start() {

}
void MQTTAbstraction::stop() {
}

void MQTTAbstraction::connect() {
	if(!WiFi.isConnected())
		return;

	if( !this->client.connected()) {
		Serial.printf("> Trying to connect to MQTT broker at %s\n", broker.c_str() );

		if(this->client.connect(WiFi.macAddress().c_str(), "try", "try")) {
			Serial.printf("> MQTT Broker connected at %s\n", this->broker.c_str());
			this->client.subscribe( WiFi.macAddress() + CONFIG_TOPIC );
		}
		else
			Serial.println("> Unable to connect to MQTT Broker");
	}
}

void MQTTAbstraction::publishConfig(String data) {
	this->client.publish( WiFi.macAddress() + RETURN_TOPIC, data.c_str() );
}

void MQTTAbstraction::parser(String &topic, String &payload) {
	this->callCallback(payload);
}

void MQTTAbstraction::publish(const char *topic, const char *data, int length)
{
	if(!WiFi.isConnected())
		return;

	this->client.publish( (const char*) topic, data, length );
}

void MQTTAbstraction::publish(const char *topic, const char *data) {
	if(!WiFi.isConnected())
		return;
	this->client.publish( topic, data);
}

void MQTTAbstraction::loop() {
    this->client.loop();

	if(!this->client.connected())
		this->connect();
}