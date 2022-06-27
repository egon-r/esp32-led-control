//
// Created by skynet on 11/18/2020.
//

#include "WebServer.h"
#include "SPIFFS.h"


WebServer::WebServer(SystemSettings *settings) {
    this->settings = settings;
    server = new AsyncWebServer(settings->WEBSERVER_PORT);
}

void WebServer::Start() {
    Serial.println("Starting WebServer ...");
    //Serial.println("SPIFFS: " + String((int)SPIFFS.usedBytes(), 10) + " / " + String((int)SPIFFS.totalBytes(), 10));

    server->serveStatic("/", SPIFFS, "/www/")
        .setDefaultFile("index.html")
        .setTemplateProcessor([this](const String& var) -> String {
            if(var == "WIFI_SSID") {
                return String(settings->WIFI_SSID);
            } else if(var == "SPIFFS_USED") {
                return String(String((int)SPIFFS.usedBytes()));
            } else if(var == "SPIFFS_TOTAL") {
                return String(String((int)SPIFFS.totalBytes()));
            } else {
                return String();
            }
        });

    server->on("/led/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        //TODO: implement json api to control attached WS2815 led strip
        Serial.println("GET: /led/");
        Serial.println("ARGS:");
        for(int i = 0; i < request->args(); i++) {
            auto argName = request->argName(i);
            auto argValue = request->arg(i);
            Serial.println("\t" + argName + " -> " + argValue);
            if(argName == "brightness") {
                settings->LED_STRIP_01_BRIGHTNESS = argValue.toInt();
                settings->LED_STRIP_02_BRIGHTNESS = argValue.toInt();
            } else if(argName == "mode") {
                if(argValue == "OFF_CUSTOM") {
                    settings->ledMode = SystemSettings::OFF_CUSTOM;
                } else if(argValue == "RAINBOW") {
                    settings->ledMode = SystemSettings::RAINBOW;
                }
            }
        }
        request->send(200);
    });

    server->onNotFound([](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "Not Found!");
    });
    server->begin();
}


WebServer::~WebServer() {
    delete server;
}
