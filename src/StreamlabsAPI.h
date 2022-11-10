#include <Arduino.h>
#include <map>
#include <SocketIOclient.h>

#ifdef DEBUG_ESP_PORT
#define DEBUG_STREAMLABS_ALERTS(...)               \
    {                                       \
        DEBUG_ESP_PORT.printf(__VA_ARGS__); \
        DEBUG_ESP_PORT.flush();             \
    }
#else
#define DEBUG_STREAMLABS_ALERTS(...)
#endif

#define SL_HOSTNAME "sockets.streamlabs.com"
#define SL_PORT 443
#define SL_DEFAULT_QUERYPARAMETES "/socket.io/?"


#ifndef STREAMLABSAPI_H_
#define STREAMLABSAPI_H_

class StreamlabsAPI {
    private:
        SocketIOclient webSocket;
	    std::map<String, std::function<void (const char * payload)>> events;
        void event(socketIOmessageType_t type, uint8_t * payload, size_t length);
    public:
        void loop();
        void connect(const char* socketToken);
        void followTwitchEvent(std::function<void (const char * payload)> func);
        void subscriptionsTwitchEvent(std::function<void (const char * payload)> func);
        void resubscriptionsTwitchEvent(std::function<void (const char * payload)> func);
        void hostTwitchEvent(std::function<void (const char * payload)> func);
        void bitsTwitchEvent(std::function<void (const char * payload)> func);
        void raidsTwitchEvent(std::function<void (const char * payload)> func);
        void donationEvent(std::function<void (const char * payload)> func);
};


#endif /* STREAMLABSAPI_H_ */