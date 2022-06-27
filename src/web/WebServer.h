//
// Created by skynet on 11/18/2020.
//

#ifndef REMOTE_ADMIN_WEBSERVER_H
#define REMOTE_ADMIN_WEBSERVER_H

#include <settings/SystemSettings.h>
#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

class WebServer {
private:
    AsyncWebServer *server;
    SystemSettings *settings;

public:
    WebServer(SystemSettings *settings);
    virtual ~WebServer();

    void Start();
};


#endif //REMOTE_ADMIN_WEBSERVER_H
