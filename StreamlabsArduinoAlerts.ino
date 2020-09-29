#include <Arduino.h>
#include <Esp.h>
#include <SocketIoClient.h>
#include <ESP8266WiFi.h>
#include "config.h"

SocketIoClient webSocket;


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

    String a = "/socket.io/?transport=websocket&EIO=3&token=" + access_token;
    char queryParameters[400];
    a.toCharArray(queryParameters, 400, 0);

    Serial.println("Non ce la farai mai");

    webSocket.on("event", event);
    webSocket.begin("aws-io.streamlabs.com", 443, queryParameters);//, "CF DC 28 BA 68 80 F0 E5 9E CC 81 6F AB 0A AB 66 AE D9 68 1C"); //"E7:93:77:36:DA:D4:15:0F:C1:C1:8F:14:D5:2A:C8:72:93:D0:6F:2A");
    Serial.println("begin finito");
}

void loop() {
    webSocket.loop();
}
