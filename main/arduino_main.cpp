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
#include "esc/ESC.h"

#define DEADZONE 50
#define LEFTESC_WHITE_PIN 4
#define RIGHTESC_WHITE_PIN 14

#define ESC_CENTER_POINT 1500
#define ESC_LOW_POINT 1100
#define ESC_HIGH_POINT 1900
// #define ESC_LOW_POINT (ESC_CENTER_POINT - 100)
// #define ESC_HIGH_POINT (ESC_CENTER_POINT + 100)

ESC LEFT_ESC = ESC(LEFTESC_WHITE_PIN);
ESC RIGHT_ESC = ESC(RIGHTESC_WHITE_PIN);

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.

static GamepadPtr myGamepad;

int prevLeft = ESC_CENTER_POINT;
int prevRight = ESC_CENTER_POINT;

void onConnectedGamepad(GamepadPtr gp) {
    myGamepad = gp;
    Serial.println("CALLBACK: Gamepad is connected!");
}

void onDisconnectedGamepad(GamepadPtr gp) {
    Serial.println("CALLBACK: Gamepad is disconnected!");
    myGamepad = nullptr;
}

// Arduino setup function. Runs in CPU 1
void setup() {
    Serial.begin(115200);

    String fv = BP32.firmwareVersion();
    Serial.print("Firmware: ");
    Serial.println(fv);

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

    pinMode(LEFTESC_WHITE_PIN, OUTPUT);
    pinMode(RIGHTESC_WHITE_PIN, OUTPUT);

    LEFT_ESC.arm();
    RIGHT_ESC.arm();
}

// TODO: Add Axis scaling for unbalanced sticks
// Axis -511 to 512
// esc 1100 to 1900
double scaleToESC(int axisValue) {
    double lowRange = (ESC_CENTER_POINT - ESC_LOW_POINT);
    double highRange = (ESC_HIGH_POINT - ESC_CENTER_POINT);
    if (axisValue < 0) {
        auto scalar = axisValue / -511.;
        return ESC_CENTER_POINT - (scalar * lowRange);
    }
    if (axisValue > 0) {
        auto scalar = axisValue / 512.;
        return ESC_CENTER_POINT + (scalar * highRange);
    }

    return double(ESC_CENTER_POINT);
}

int getSpeed(int axis, bool forward, bool backward, int throttleValue, int model) {
    if (forward && backward) {
        return 0;
    }
    if (forward) {
        // Throttle reports as 0 for controllers without analog
        if (model == Gamepad::CONTROLLER_TYPE_WiiController || model == Gamepad::CONTROLLER_TYPE_SwitchProController ||
            model == Gamepad::CONTROLLER_TYPE_SwitchJoyConLeft || model == Gamepad::CONTROLLER_TYPE_SwitchJoyConRight ||
            model == Gamepad::CONTROLLER_TYPE_SwitchJoyConPair) {
            return 512;
        }
        return min(int(ceil(throttleValue / 2)), 512);
    }
    if (backward) {
        return -511;
    }
    // Use this to account for
    if (abs(axis) < DEADZONE) {
        return 0;
    }
    // I just want forward to be positive and backwards to be negative, so I invert the values here
    return -axis;
}

double makeNice(double target, double current) {
    double leftToGo = abs(target - current);

    double newChange = min(1., leftToGo);

    return (target > current ? current + newChange : current - newChange);
}

void handleThrottle(GamepadPtr gp) {
    int leftSpeed = gp->axisY();
    int rightSpeed = gp->axisRY();

    int leftThrottle = gp->brake();
    int rightThrottle = gp->throttle();

    bool l1 = gp->l1();
    bool l2 = gp->l2();
    bool r1 = gp->r1();
    bool r2 = gp->r2();

    leftSpeed = getSpeed(leftSpeed, l2, l1, leftThrottle, gp->getModel());
    rightSpeed = getSpeed(rightSpeed, r2, r1, rightThrottle, gp->getModel());

    double leftPWM = scaleToESC(leftSpeed);
    double rightPWM = scaleToESC(rightSpeed);

    double leftLinearPWM = makeNice(leftPWM, prevLeft);
    double rightLinearPWM = makeNice(rightPWM, prevRight);

    // Serial.println("Left\t" + String(leftThrottle) + "(" + l2 + ")" + "\tRight\t" + String(rightThrottle) + "(" + r2
    // +
    //                ")");
    // Serial.println("LS\t" + String(leftSpeed) + "(" + l2 + ")" + "\tRS\t" + String(rightSpeed) + "(" + r2 + ")");

    Serial.println("ESCL\t" + String(leftLinearPWM) + "(" + leftPWM + ")" + "\tESCR\t" + String(rightLinearPWM) + "(" +
                   rightPWM + ")");

    LEFT_ESC.spin(leftLinearPWM);
    RIGHT_ESC.spin(rightLinearPWM);

    prevLeft = leftLinearPWM;
    prevRight = rightLinearPWM;
}

void loop() {
    BP32.update();
    if (myGamepad && myGamepad->isConnected()) {
        handleThrottle(myGamepad);
    }
    delay(2);
}
