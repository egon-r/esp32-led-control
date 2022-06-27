//
// Created by skynet on 11/18/2020.
//

#ifndef REMOTE_ADMIN_SYSTEMSETTINGS_H
#define REMOTE_ADMIN_SYSTEMSETTINGS_H

#include <Arduino.h>
#include <WiFi.h>
#include "SPIFFS.h"
#include "ESPmDNS.h"


class SystemSettings {
public:
    enum LED_MODE {
        OFF_CUSTOM,
        RAINBOW
    };

    // not yet configurable settings
    const int ONBOARD_LED = 2;
    const int BAUD_RATE = 9600;
    const int WEBSERVER_PORT = 80;
    const int LED_STRIP_01_COUNT = 60;
    const int LED_STRIP_01_DATA = 18;
    const int LED_STRIP_02_COUNT = 240;
    const int LED_STRIP_02_DATA = 19;

    // configurable settings
    char* WIFI_SSID = (char*)"ssid";
    char* WIFI_PASS = (char*)"password";
    LED_MODE ledMode = OFF_CUSTOM;
    uint8_t LED_STRIP_01_BRIGHTNESS = 0;
    uint8_t LED_STRIP_02_BRIGHTNESS = 0;

    void InitializeHardware() {
        Serial.begin(BAUD_RATE);
        Serial.printf("Serial ready! Baud: %i\n", BAUD_RATE);
        pinMode(ONBOARD_LED, OUTPUT);

        Serial.println("Initializing SPIFFS ...");
        if(!SPIFFS.begin(true)) {
            Serial.println("Error mounting SPIFFS!");
        } else {
            File file = SPIFFS.open("/test.txt");
            if(!file) {
                Serial.println("Failed to open /test.txt on SPIFFS!");
            } else {
                Serial.println("Reading: /test.txt");
                while(file.available()) {
                    Serial.write(file.read());
                }
                Serial.println("\n");
                file.close();
                Serial.println("SPIFFS: " + String((int)SPIFFS.usedBytes(), 10) + " / " + String((int)SPIFFS.totalBytes(), 10));
            }
        }

        Serial.println("Initializing Wifi...");
        WiFi.mode(WIFI_STA);
        WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
        TryConnectWifi();
    }

    void TryConnectWifi() {
        if(WiFi.isConnected()) {
            WiFi.disconnect();
        }
        WiFi.setHostname("myesp32");
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        Serial.printf("Connecting to network '%s' ...\n", WIFI_SSID);
        if(WiFi.waitForConnectResult() != WL_CONNECTED) {
            Serial.printf("Error connecting to '%s' ...\n", WIFI_SSID);
        } else {
            Serial.printf("Connected to '%s'!\n", WIFI_SSID);
            PrintWifiDetails();
            if(!MDNS.begin("myesp32")) {
                Serial.println("Failed to start MDNS...");
            }
        }
    }

    void PrintWifiDetails() {
        Serial.printf("MAC: %s\n", WiFi.macAddress().c_str());
        Serial.printf("IPv4: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("IPv6: %s\n", WiFi.localIPv6().toString().c_str());
        Serial.printf("RSSI: %i\n", WiFi.RSSI());
        Serial.printf("Channel: %i\n", WiFi.channel());
    }
};


#endif //REMOTE_ADMIN_SYSTEMSETTINGS_H
