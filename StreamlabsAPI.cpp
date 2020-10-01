#include "StreamlabsAPI.h"
#include <ArduinoJson.h>


void StreamlabsAPI::loop() {
    webSocket.loop();
}

bool StreamlabsAPI::connect(const char* socketToken) {
    webSocket.on("event", std::bind(&StreamlabsAPI::event, this, std::placeholders::_1, std::placeholders::_2));    

    String queryParams = SL_DEFAULT_QUERYPARAMETES;
    queryParams += "&token=";
    queryParams += socketToken;
    webSocket.beginSSL(SL_HOSTNAME, SL_PORT, queryParams.c_str(), SL_FINGERPRINT);
}


void StreamlabsAPI::followTwitchEvent(std::function<void (const char * payload)> func) {
    events["follow"] = func;
}

void StreamlabsAPI::event(const char * payload, size_t length) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
        Serial.print("deserializeJson() failed: ");
        return;
    }

    const char* type = doc["type"];
	auto e = events.find(type);
	if(e != events.end()) {
		e->second(payload);
    }
}