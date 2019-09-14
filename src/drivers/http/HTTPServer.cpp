#include "HTTPServer.h"

HTTPServer::HTTPServer() : server(SERVER_PORT) { 

	server.on("/", HTTP_POST, [](AsyncWebServerRequest *request) {}, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
		Serial.printf("[POST]: %s\n", request->url().c_str() );

		// TODO - Cut the data char vector to the len size ending with '\0'
		if(this->onDataCallback && len)
			this->onDataCallback( String( (char*) data) );

		request->send(200, "OK");
	});
	
}

HTTPServer::~HTTPServer() { }

void HTTPServer::start() {
	server.begin();
}

void HTTPServer::stop() {
	server.end();
}

void HTTPServer::registerOnDataCallback( std::function<void(String)> callback ) {
	this->onDataCallback = callback;
}
