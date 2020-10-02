/*
 * StreamlabsArduinoAlerts.ino
 *
 *  Created on: 02/10/2020
 *  Author: Luca Taddeo
 *
 */

#include <Arduino.h>
#include <Esp.h>
#include <ESP8266WiFi.h>
#include <StreamlabsAPI.h>

#define SSID "ssid"
#define PASSWORD "password"
#define SOCKET_TOKEN "token"

StreamlabsAPI streamlabsAPI;

void followerEvent(const char * payload) {
  Serial.print("got followerEvent message: ");
  Serial.println(payload);
}

void subscriptionEvent(const char * payload) {
  Serial.print("got subscriptionEvent message: ");
  Serial.println(payload);
}

void bitsEvent(const char * payload) {
  Serial.print("got bitsEvent message: ");
  Serial.println(payload);
}

void donationsEvent(const char * payload) {
  Serial.print("got donationsEvent message: ");
  Serial.println(payload);
}

void setup() {
    
    Serial.begin(115200);
    delay(1000);


    WiFi.begin(SSID, PASSWORD);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(++i); Serial.print(' ');
    }

    Serial.println('\n');
    Serial.println("Connection established!");  
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP()); 

    streamlabsAPI.followTwitchEvent(followerEvent);
    streamlabsAPI.subscriptionsTwitchEvent(subscriptionEvent);
    streamlabsAPI.bitsTwitchEvent(bitsEvent);
    streamlabsAPI.donationEvent(donationsEvent);
    streamlabsAPI.connect(SOCKET_TOKEN);
}

void loop() {
    streamlabsAPI.loop();
}
