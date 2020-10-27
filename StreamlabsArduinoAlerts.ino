#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "src/StreamlabsAPI.h"
#include "config.h"

StreamlabsAPI streamlabsAPI("","","");

void event(const char * payload, size_t length) {
  Serial.print("got message: ");
  Serial.println(payload);
}

void handler(const char * payload, size_t length) {
  Serial.print("Connected: ");
  Serial.println(payload);
}

void handlerDisc(const char * payload, size_t length) {
  Serial.print("Disconnected: ");
  Serial.println(payload);
}

void setup() {
    
    Serial.begin(115200);
    delay(1000);


    WiFi.begin(ssid, wifiPwd);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
      delay(1000);
      Serial.print(++i); Serial.print(' ');
    }

    Serial.println('\n');
    Serial.println("Connection established!");  
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP()); 

    String queryParameters = "/socket.io/?transport=websocket&token=" + access_token;

    Serial.println("Non ce la farai mai");

    streamlabsAPI.connect();
    Serial.println("begin finito");
}

void loop() {
  streamlabsAPI.loop();
}
