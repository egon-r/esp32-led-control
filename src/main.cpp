#include <Arduino.h>
#include <Wire.h>
#include "SSD1306Wire.h"
#include <web/OTAUpdater.h>
#include <web/WebServer.h>
#include <Adafruit_NeoPixel.h>

SystemSettings *settings = new SystemSettings();
Adafruit_NeoPixel *strip = new Adafruit_NeoPixel(settings->LED_STRIP_01_COUNT, settings->LED_STRIP_01_DATA, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel *strip2 = new Adafruit_NeoPixel(settings->LED_STRIP_02_COUNT, settings->LED_STRIP_02_DATA, NEO_GRB + NEO_KHZ800);
WebServer *webServer = new WebServer(settings);
OTAUpdater *otaUpdater = new OTAUpdater(settings, []() {});

TaskHandle_t Task1;
TaskHandle_t Task2;

[[noreturn]] void Task1code(void *pvParameters) {
    for (;;) {
        otaUpdater->CheckForUpdate();
        delay(500);
    }
}

[[noreturn]] void Task2code(void *pvParameters) {
    long firstPixelHue = 0;

    for (;;) {
        if (otaUpdater->isUpdating) {
            strip->clear();
            strip->show();
            strip2->clear();
            strip2->show();
            delay(100);
        } else {
            switch (settings->ledMode) {
                case SystemSettings::OFF_CUSTOM:
                    strip->clear();
                    strip2->clear();
                    break;
                case SystemSettings::RAINBOW:
                    if (firstPixelHue < 5 * 65536) {
                        firstPixelHue += 256;
                    } else {
                        firstPixelHue = 0;
                    }
                    for (int i = 0; i < strip->numPixels(); i++) {
                        int pixelHue = firstPixelHue + (i * 65536L / strip->numPixels());
                        strip->setPixelColor(i, Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(pixelHue))); // gamma32 = gamma correction for smoother color transition
                    }
                    for (int i = 0; i < strip2->numPixels(); i++) {
                        int pixelHue = firstPixelHue + (i * 65536L / strip2->numPixels());
                        strip2->setPixelColor(i, Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(pixelHue))); // gamma32 = gamma correction for smoother color transition
                    }
                    delay(50);
                    break;
            }
            strip->setBrightness(settings->LED_STRIP_01_BRIGHTNESS);
            strip2->setBrightness(settings->LED_STRIP_02_BRIGHTNESS);
            strip->show();
            strip2->show();
        }
    }
}

void setup() {
    // init system (esp32)
    settings->InitializeHardware();
    otaUpdater->Init();

    xTaskCreatePinnedToCore(
            Task1code,   // Task function.
            "Task1",     // name of task.
            10000,       // Stack size of task
            NULL,        // parameter of the task
            1,           // priority of the task
            &Task1,      // Task handle to keep track of created task
            0   // pin task to core 0
    );
    //delay(500);
    xTaskCreatePinnedToCore(
            Task2code,
            "Task2",
            10000,
            NULL,
            1,
            &Task2,
            1
    );
    webServer->Start();
}

void loop() {
    delay(1000);
}