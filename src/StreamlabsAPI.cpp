#include "StreamlabsAPI.h"


void StreamlabsAPI::loop() {
    webSocket.loop();
}

void StreamlabsAPI::connect(const char* socketToken) {
    webSocket.onEvent(std::bind(&StreamlabsAPI::event, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    String queryParams = SL_DEFAULT_QUERYPARAMETES;
    queryParams.concat("token=");
    queryParams.concat(socketToken);
    webSocket.beginSSL(SL_HOSTNAME, SL_PORT, queryParams.c_str());
}

void StreamlabsAPI::followTwitchEvent(std::function<void (const char * payload)> func) {
    events["follow"] = func;
}

void StreamlabsAPI::subscriptionsTwitchEvent(std::function<void (const char * payload)> func) {
    events["subscription"] = func;
}

void StreamlabsAPI::hostTwitchEvent(std::function<void (const char * payload)> func) {
    events["host"] = func;
}

void StreamlabsAPI::bitsTwitchEvent(std::function<void (const char * payload)> func) {
    events["bits"] = func;
}

void StreamlabsAPI::raidsTwitchEvent(std::function<void (const char * payload)> func) {
    events["raid"] = func;
}

void StreamlabsAPI::donationEvent(std::function<void (const char * payload)> func) {
    events["donation"] = func;
}

void StreamlabsAPI::event(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    const char* data = reinterpret_cast<char*>(payload);
    DEBUG_STREAMLABS_ALERTS("------STREAMLABS EVENT------\n");
    DEBUG_STREAMLABS_ALERTS("SocketIO message type: %d\n", type);
    DEBUG_STREAMLABS_ALERTS("SocketIO data: %s\n", data);

    String payloadS = String((char *) payload);
    int startOfType = payloadS.indexOf("\"type\":\"");
    int endOfType = payloadS.indexOf("\"", startOfType + 8);
    String typeS = payloadS.substring(startOfType + 8, endOfType);

    DEBUG_STREAMLABS_ALERTS("Streamlabs event type: %s\n", typeS.c_str());
    DEBUG_STREAMLABS_ALERTS("-----------------------------\n");

	auto e = events.find(typeS);
	if(e != events.end()) {
		e->second(data);
    }
}