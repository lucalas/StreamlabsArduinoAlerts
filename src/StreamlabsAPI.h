#ifndef STREAMLABSAPI_H_
#define STREAMLABSAPI_H_
#include <Arduino.h>

#include <map>
#include <SocketIoClient.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

#define STREAMLABSAPI_DEBUG(...) Serial.printf(__VA_ARGS__);

#define SL_HOSTNAME "sockets.streamlabs.com"
#define SL_PORT 443
#define SL_DEFAULT_QUERYPARAMETERS "/socket.io/?transport=websocket"
#define SL_FINGERPRINT "E7 93 77 36 DA D4 15 0F C1 C1 8F 14 D5 2A C8 72 93 D0 6F 2A"

class StreamlabsAPI {
    private:
        SocketIoClient webSocket;
        String clientId;
        String clientSecret;
        String redirectUri;
        const char* accessToken;
        const char* socketToken;
	    std::map<String, std::function<void (const char * payload)>> events;
        void event(const char * payload, size_t length);
        void getHTTPClient();
        void closeHTTPClient();
        void getStreamlabsCode();
        int getSocketToken();
        int getAccessToken();
        void startSocketConnection(String socketToken);
    public:
        StreamlabsAPI(String client_id, String client_secret, String redirect_uri);
        void loop();
        bool connect();
        void followTwitchEvent(std::function<void (const char * payload)> func);
        void subscriptionsTwitchEvent(std::function<void (const char * payload)> func);
        void hostTwitchEvent(std::function<void (const char * payload)> func);
        void bitsTwitchEvent(std::function<void (const char * payload)> func);
        void raidsTwitchEvent(std::function<void (const char * payload)> func);
        void donationEvent(std::function<void (const char * payload)> func);
};

#endif /* STREAMLABSAPI_H_ */