#include "utils.h"
#include <Arduino.h>
#include <Bluepad32.h>
#include "controls/stickdata.h"

TaskHandle_t* blinkerTask = nullptr;

void utils::blinker(void* params) {
    auto gp = ((GamepadPtr)params);
    for (int i = 0; i < 10; i++) {
        int cLed = i % 2 ? 255 : 0;
        int pLed = i % 2 ? 1 : 0;

        gp->setColorLED(cLed, cLed, cLed);
        gp->setPlayerLEDs(pLed);
        delay(100);
    }
    vTaskDelete(NULL);
}

void utils::blinkController(GamepadPtr gp) {
    if (blinkerTask != nullptr) {
        delete blinkerTask;
    }
    blinkerTask = new TaskHandle_t();
    xTaskCreatePinnedToCore(blinker,     /* Function to implement the task */
                            "blinker",   /* Name of the task */
                            10000,       /* Stack size in words */
                            (void*)gp,   /* Task input parameter */
                            0,           /* Priority of the task */
                            blinkerTask, /* Task handle. */
                            0);
}

void utils::calibrateSticks(GamepadPtr gp, StickData* stickData) {
    int leftAxisY = -gp->axisY();
    int rightAxisY = -gp->axisRY();
    int leftAxisX = gp->axisX();
    int rightAxisX = gp->axisRX();

    if (leftAxisY < 0) {
        stickData->minY = min(leftAxisY, stickData->minY);
    } else {
        stickData->maxY = max(leftAxisY, stickData->maxY);
    }
    if (rightAxisY < 0) {
        stickData->minRY = min(rightAxisY, stickData->minRY);
    } else {
        stickData->maxRY = max(rightAxisY, stickData->maxRY);
    }
    if (leftAxisX < 0) {
        stickData->minX = min(leftAxisX, stickData->minX);
    } else {
        stickData->maxX = max(leftAxisX, stickData->maxX);
    }
    if (rightAxisX < 0) {
        stickData->minRX = min(rightAxisX, stickData->minRX);
    } else {
        stickData->maxRX = max(rightAxisX, stickData->maxRX);
    }
}