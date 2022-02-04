/****************************************************************************
http://retro.moe/unijoysticle2

Copyright 2021 Ricardo Quesada

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
****************************************************************************/

#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>
#include "accel/accel.h"
#include "accel/linear.h"
#include "controls/controlscheme.h"
#include "controls/twinstick.h"
#include "esc/ESC.h"
#include "utils.h"

#define DEADZONE 50
#define LEFTESC_WHITE_PIN 4
#define RIGHTESC_WHITE_PIN 14

#define ESC_CENTER_POINT 1500
#define ESC_LOW_POINT 1100
#define ESC_HIGH_POINT 1900
// #define ESC_LOW_POINT (ESC_CENTER_POINT - 100)
// #define ESC_HIGH_POINT (ESC_CENTER_POINT + 100)

ESC LEFT_ESC = ESC(LEFTESC_WHITE_PIN, ESC_LOW_POINT, ESC_HIGH_POINT, ESC_CENTER_POINT);
ESC RIGHT_ESC = ESC(RIGHTESC_WHITE_PIN, ESC_LOW_POINT, ESC_HIGH_POINT, ESC_CENTER_POINT);

static GamepadPtr myGamepad;
static ControlScheme* controlScheme;
static Accel* acceleration = new Linear();

int prevLeft = ESC_CENTER_POINT;
int prevRight = ESC_CENTER_POINT;
int maxXY = 511;
int maxRXY = 511;
int minXY = -512;
int minRXY = -512;
bool isCalibrating = false;

TaskHandle_t* controllerTask = nullptr;

void onDisconnectedGamepad(GamepadPtr gp) {
    Serial.println("CALLBACK: Gamepad is disconnected!");
    myGamepad = nullptr;
    LEFT_ESC.disarm();
    RIGHT_ESC.disarm();

    vTaskDelete(*controllerTask);
}

void handleThrottle(void* params) {
    for (;;) {
        if (isCalibrating) {
            continue;
        }
        if (myGamepad && controlScheme && myGamepad->isConnected()) {
            auto axis = controlScheme->handleThrottle();
            int leftSpeed = axis.first;
            int rightSpeed = axis.second;

            double leftPWM = LEFT_ESC.getScaler()->scaleAxis(leftSpeed);
            double rightPWM = RIGHT_ESC.getScaler()->scaleAxis(rightSpeed);

            double leftLinearPWM = acceleration->accelerate(leftPWM, prevLeft);
            double rightLinearPWM = acceleration->accelerate(rightPWM, prevRight);

            Serial.println("ESCL\t" + String(leftLinearPWM) + "(" + leftPWM + ")" + "\tESCR\t" +
                           String(rightLinearPWM) + "(" + rightPWM + ")");

            LEFT_ESC.spin(leftLinearPWM);
            RIGHT_ESC.spin(rightLinearPWM);

            prevLeft = leftLinearPWM;
            prevRight = rightLinearPWM;
        }
        delay(1);
    }
}

void createControllerTask(GamepadPtr gp) {
    if (controllerTask != nullptr) {
        delete controllerTask;
    }
    controllerTask = new TaskHandle_t();
    xTaskCreatePinnedToCore(handleThrottle, /* Function to implement the task */
                            "throttle",     /* Name of the task */
                            10000,          /* Stack size in words */
                            NULL,           /* Task input parameter */
                            0,              /* Priority of the task */
                            controllerTask, /* Task handle. */
                            0);
}

void onConnectedGamepad(GamepadPtr gp) {
    if (myGamepad != nullptr) {
        return;
    }
    createControllerTask(gp);

    myGamepad = gp;
    controlScheme = new TwinStick(myGamepad, DEADZONE);
    Serial.println("CALLBACK: Gamepad is connected!");
    LEFT_ESC.arm();
    RIGHT_ESC.arm();
}

void calibrateSticks(GamepadPtr gp) {
    int leftAxis = -gp->axisY();
    int rightAxis = -gp->axisRY();
    if (leftAxis < 0) {
        minXY = min(leftAxis, minXY);
    } else {
        maxXY = max(leftAxis, maxXY);
    }
    if (rightAxis < 0) {
        minRXY = min(rightAxis, minRXY);
    } else {
        maxRXY = max(rightAxis, maxRXY);
    }
}

void setup() {
    Serial.begin(115200);

    String fv = BP32.firmwareVersion();
    Serial.print("Firmware: ");
    Serial.println(fv);

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    pinMode(LEFTESC_WHITE_PIN, OUTPUT);
    pinMode(RIGHTESC_WHITE_PIN, OUTPUT);
    delay(250);
}

void loop() {
    BP32.update();
    if (myGamepad && myGamepad->isConnected()) {
        if (!isCalibrating) {
            // handleThrottle(myGamepad);
            if (myGamepad->a() && myGamepad->b()) {
                utils::blinkController(myGamepad);
                isCalibrating = true;
                maxXY = maxRXY = minXY = minRXY = 0;
            }
        } else {
            calibrateSticks(myGamepad);
            if (myGamepad->x() && myGamepad->y()) {
                utils::blinkController(myGamepad);
                isCalibrating = false;
                Serial.println("newVals: ");
                Serial.println("maxXY: " + String(maxXY));
                Serial.println("maxRXY: " + String(maxRXY));
                Serial.println("minXY: " + String(minXY));
                Serial.println("minRXY: " + String(minRXY));
            }
        }
    }
    delay(150);
}
