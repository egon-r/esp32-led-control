//
// Created by skynet on 11/17/2020.
//

#ifndef REMOTE_ADMIN_OTAUPDATER_H
#define REMOTE_ADMIN_OTAUPDATER_H

#include <Arduino.h>
#include "SSD1306Wire.h"
#include <settings/SystemSettings.h>

class OTAUpdater {
public:
    SystemSettings *settings;
    bool isUpdating;
    float currentProgress;
    void (*onUpdate)();

    OTAUpdater(SystemSettings* settings, void (*onUpdate)());

    void CheckForUpdate();
    void Init();
};


#endif //REMOTE_ADMIN_OTAUPDATER_H
