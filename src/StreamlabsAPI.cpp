#include "StreamlabsAPI.h"


String code;
ESP8266WebServer webServer(80);
StaticJsonDocument<10000> doc;
std::unique_ptr<BearSSL::WiFiClientSecure>webClient(new BearSSL::WiFiClientSecure);
const uint8_t fingerprint[20] = {0xcf, 0xdc, 0x28, 0xba, 0x68, 0x80, 0xf0, 0xe5, 0x9e, 0xcc, 0x81, 0x6f, 0xab, 0x0a, 0xab, 0x66, 0xae, 0xd9, 0x68, 0x1c};

StreamlabsAPI::StreamlabsAPI(String client_id, String client_secret, String redirect_uri) {
    clientId = client_id;
    clientSecret = client_secret;
    redirectUri = redirect_uri;
}

void StreamlabsAPI::loop() {
    webSocket.loop();
}

void StreamlabsAPI::startSocketConnection(String socketToken) {
    STREAMLABSAPI_DEBUG("Starting WebSocket connection\n");
    webSocket.on("event", std::bind(&StreamlabsAPI::event, this, std::placeholders::_1, std::placeholders::_2));    

    String queryParams = SL_DEFAULT_QUERYPARAMETERS;
    queryParams += "&token=";
    queryParams += socketToken;
    webSocket.beginSSL(SL_HOSTNAME, SL_PORT, queryParams.c_str(), SL_FINGERPRINT);
    STREAMLABSAPI_DEBUG("WebSocket connection started\n");
}

bool StreamlabsAPI::connect() {
    STREAMLABSAPI_DEBUG("Connecting...\n");
    // Start server to retrieve code
    getStreamlabsCode();

    // exec flow to get socket token
    getHTTPClient();
    int accessTokenErrorCode = getAccessToken();
    if (accessTokenErrorCode != 200) {
        STREAMLABSAPI_DEBUG("Access Token get error code: %s\n", accessTokenErrorCode);
        // TODO remove return and add retry
        return -1;
    }

    int socketTockenErrorCode = getSocketToken();
    if (socketTockenErrorCode != 200) {
        STREAMLABSAPI_DEBUG("Socket Token get error code: %d\n", socketTockenErrorCode);
        // TODO remove return and add retry
        return -1;
    }
    closeHTTPClient();

    // call startSocketConnection(socketToken)
    startSocketConnection(socketToken);
    STREAMLABSAPI_DEBUG("Connected.\n");
}

void StreamlabsAPI::getStreamlabsCode() {
    webServer.on("/code", []() {
        code = webServer.arg("code");
        webServer.send(200, "text/plain", "Ok");
        STREAMLABSAPI_DEBUG("Code Retrieved\n");
    });
    webServer.onNotFound([]() {
        webServer.send(200, "text/plain", "Invalid request");
    });

    STREAMLABSAPI_DEBUG("Starting Server to retrieve Code\n");
    webServer.begin();
    STREAMLABSAPI_DEBUG("Started Server to retrieve Code\n");

    while (code == NULL) {
        webServer.handleClient();
        if (code != NULL) {
            STREAMLABSAPI_DEBUG("Code retrieved\n");
            webServer.stop();
            // TODO Verify if destructor is required for out of memory
            //webServer.~ESP8266WebServerTemplate();
            STREAMLABSAPI_DEBUG("Web server stopped\n");
        }
    }
    
}

void StreamlabsAPI::getHTTPClient() {
    STREAMLABSAPI_DEBUG("Creating HTTPClient\n");
    //std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    webClient->setFingerprint(fingerprint);
    
    STREAMLABSAPI_DEBUG("Created HTTPClient\n");
}

void StreamlabsAPI::closeHTTPClient() {
    STREAMLABSAPI_DEBUG("Stopping HTTPClient\n");
    webClient->stopAll();
    STREAMLABSAPI_DEBUG("Stopped HTTPClient\n");
}

int StreamlabsAPI::getAccessToken() {
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);
    const uint8_t fingerprint[20] = {0XCF, 0XDC, 0X28, 0XBA, 0X68, 0X80, 0XF0, 0XE5, 0X9E, 0XCC, 0X81, 0X6F, 0XAB, 0X0A, 0XAB, 0X66, 0XAE, 0XD9, 0X68, 0X1C};
    client->setFingerprint(fingerprint);

    STREAMLABSAPI_DEBUG("Starting connection to https://streamlabs.com/api/v1.0/token\n");
    //client.setFingerprint(fingerprint);

    if (!client->connect("streamlabs.com", 443)) {
        STREAMLABSAPI_DEBUG("Connection Failed");
        return -1;
    }
    STREAMLABSAPI_DEBUG("Connected to webClient to get AccessToken");

    String data = "grant_type=authorization_code&client_id=" + clientId + "&client_secret=" + clientSecret + "&redirect_uri=" + redirectUri + "&code=" + code; 

    client->print(String("POST /api/v1.0/token HTTP/1.1\r\n") +
            "Host: streamlabs.com\r\n" +
            "Accept: *" + "/" + "*\r\n" +
            "User-Agent: ESP8266\r\n" +
            "Content-Length: " + data.length() + "\r\n" +
            "Content-Type: aplication/x-www-form-urlencoded\r\n" +
            "\r\n" + data);

    while (client->connected()) {
        String line = client->readStringUntil('\n');
        if (line == "\r") {
            STREAMLABSAPI_DEBUG("headers received\n");
            break;
        }
    }
    String line = client->readStringUntil('\n');
    if (line.indexOf("access_token")) {
        STREAMLABSAPI_DEBUG("AccessToken retrieved\n");
    } else {
        STREAMLABSAPI_DEBUG("Error retrieving AccessToken, error:\n");
        Serial.println(line);
    }
    //HTTPClient http;
    /*String httpReqData = "grant_type=authorization_code&client_id=" + clientId + "&client_secret=" + clientSecret + "&redirect_uri=" + redirectUri + "&code=" + code; 
    http.begin(*webClient, "https://streamlabs.com/api/v1.0/token");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.POST(httpReqData);
    if (httpCode > 0) {
        String payload = http.getString();
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
            STREAMLABSAPI_DEBUG("Json Access Token deserialization error\n");
        }
        STREAMLABSAPI_DEBUG("Access Token retrieved\n");

        accessToken = doc["access_token\n"];

    } else {
        char errorCode[3];
        itoa(httpCode, errorCode, 10);
        STREAMLABSAPI_DEBUG("Access Token http Request error %s\n", errorCode);
    }
    http.end();*/
    STREAMLABSAPI_DEBUG("Stopped connection to https://streamlabs.com/api/v1.0/token\n");
}

int StreamlabsAPI::getSocketToken() {
    STREAMLABSAPI_DEBUG("Starting connection to https://streamlabs.com/api/v1.0/socket/token\n");
    HTTPClient http;
    String httpUrl = "https://streamlabs.com/api/v1.0/socket/token?access_token=";
    httpUrl += accessToken;
    http.begin(*webClient, httpUrl);
    int httpCode = http.GET();
    if (httpCode > 0) {
        String payload = http.getString();
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
            STREAMLABSAPI_DEBUG("Json Socket Token deserialization error\n");
        }

        socketToken = doc["socket_token"];

    } else {
        char errorCode[3];
        itoa(httpCode, errorCode, 10);
        STREAMLABSAPI_DEBUG("Socket Token http Request error %s\n");
    }
    http.end();
    STREAMLABSAPI_DEBUG("Stopped connection to https://streamlabs.com/api/v1.0/socket/token\n");
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

void StreamlabsAPI::event(const char * payload, size_t length) {
    STREAMLABSAPI_DEBUG("Event received\n");
    STREAMLABSAPI_DEBUG(payload);
    STREAMLABSAPI_DEBUG("\n");
    // FIXME remove deserialized and parse string to get the type
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