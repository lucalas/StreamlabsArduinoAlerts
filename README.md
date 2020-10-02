# Streamlabs Arduino Alerts [![Build Status](https://travis-ci.com/lucalas/StreamlabsArduinoAlerts.svg?branch=master)](https://travis-ci.com/lucalas/StreamlabsArduinoAlerts)
A work in progress library to connect to your Streamlabs account and catch all the Twitch events.

## Twitch event catchable

 - Follow
 - Subscription
 - Donation
 - Raid
 - Host
 - Bit

## Supported Hardware
 - ESP8266 [Arduino for ESP8266](https://github.com/esp8266/Arduino/)
 - ESP32 [Arduino for ESP32](https://github.com/espressif/arduino-esp32)
 - ESP31B
 - Particle with STM32 ARM Cortex M3
 - ATmega328 with Ethernet Shield (ATmega branch)
 - ATmega328 with enc28j60 (ATmega branch)
 - ATmega2560 with Ethernet Shield (ATmega branch)
 - ATmega2560 with enc28j60 (ATmega branch)
 - 
## Methods
#### connect(socket_token)
Method to connect to Streamlabs server, required [socket token](https://streamlabs.readme.io/docs/sockettoken).

#### followTwitchEvent(callback)
#### subscriptionsTwitchEvent(callback)
#### hostTwitchEvent(callback)
#### bitsTwitchEvent(callback)
#### raidsTwitchEvent(callback)
#### donationEvent(callback)
#### loop()

## Example
##### Connection
```c++
char* socketToken = "token";
StreamlabsAPI streamlabsAPI.connect(socketToken);
```
##### Event Callback
```c++
void followerEvent(const char* payload) {
	Serial.print("got followerEvent message: ");
	Serial.println(payload);
}
streamlabsAPI.followTwitchEvent(followerEvent);
```

##### Loop Method
loop method is **required** to make this library work.
```c++
void loop() {
    streamlabsAPI.loop();
}
```


Event result documentation can be found on Streamlabs WebSite [HERE](https://dev.streamlabs.com/docs/socket-api)

### Issues ###
Submit issues to: https://github.com/lucalas/StreamlabsArduinoAlerts/issues