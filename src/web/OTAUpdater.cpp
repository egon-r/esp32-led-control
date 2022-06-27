//
// Created by skynet on 11/17/2020.
//

#include "OTAUpdater.h"
#include <ArduinoOTA.h>

OTAUpdater::OTAUpdater(SystemSettings *settings, void (*onUpdate)()) {
    this->settings = settings;
    this->onUpdate = onUpdate;
    this->isUpdating = false;
}

void OTAUpdater::CheckForUpdate() {
    ArduinoOTA.handle();
}

void OTAUpdater::Init() {
    Serial.println("OTAUpdater: Start()");
    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);
    // Hostname defaults to esp3232-[MAC]
    // ArduinoOTA.setHostname("myesp32");
    // No authentication by default
    // ArduinoOTA.setPassword("admin");
    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
    ArduinoOTA.onStart([this]() {
        String type;
        if(ArduinoOTA.getCommand() == U_FLASH) {
            type = "FLASH";
        } else {
            type = "SPIFFS";
        }
        Serial.println("OTA: Updating " + type + " ...");
        isUpdating = true;
        this->onUpdate();
        delay(100);
    });
    ArduinoOTA.onEnd([this](){
        Serial.println("OTA Update Done!");
        isUpdating = false;
    });
    ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total){
        //Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
        currentProgress = (progress / (total / 100.f));
        isUpdating = true;
    });
    ArduinoOTA.onError([this](ota_error_t error){
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
        isUpdating = false;
    });
    ArduinoOTA.begin();
    Serial.println("OTAUpdater initialized!");
}