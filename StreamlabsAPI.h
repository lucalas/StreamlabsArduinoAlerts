#include <Arduino.h>
#include <map>
#include <SocketIoClient.h>

#define SL_HOSTNAME "sockets.streamlabs.com"
#define SL_PORT 443
#define SL_DEFAULT_QUERYPARAMETES "/socket.io/?transport=websocket"
#define SL_FINGERPRINT "E7 93 77 36 DA D4 15 0F C1 C1 8F 14 D5 2A C8 72 93 D0 6F 2A"


#ifndef STREAMLABSAPI_H_
#define STREAMLABSAPI_H_

class StreamlabsAPI {
    private:
        SocketIoClient webSocket;
	    std::map<String, std::function<void (const char * payload)>> events;
        void event(const char * payload, size_t length);
    public:
        void loop();
        bool connect(const char* socketToken);
        void followTwitchEvent(std::function<void (const char * payload)> func);
        void subscriptionsTwitchEvent(std::function<void (const char * payload)> func);
        void hostTwitchEvent(std::function<void (const char * payload)> func);
        void bitsTwitchEvent(std::function<void (const char * payload)> func);
        void raidsTwitchEvent(std::function<void (const char * payload)> func);
        void donationEvent(std::function<void (const char * payload)> func);
};


#endif /* STREAMLABSAPI_H_ */