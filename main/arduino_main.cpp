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
#include <ESP32Servo.h>

#define DEADZONE 20

static GamepadPtr myGamepad;

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
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
}

int scaleToESC(int axisValue) {
    return 0;
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

    Serial.println("Left\t" + String(leftThrottle) + "(" + l2 + ")" + "\tRight\t" + String(rightThrottle) + "(" + r2 +
                   ")");
    Serial.println("LS\t" + String(leftSpeed) + "(" + l2 + ")" + "\tRS\t" + String(rightSpeed) + "(" + r2 + ")");
}

// Arduino loop function. Runs in CPU 1
void loop() {
    BP32.update();

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.
    if (myGamepad && myGamepad->isConnected()) {
        handleThrottle(myGamepad);

        // Another way to query the buttons, is by calling buttons(), or
        // miscButtons() which return a bitmask.
        // Some gamepads also have DPAD, axis and more.
        // snprintf(buffer, sizeof(buffer) - 1,
        //          "dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, "
        //          "%4d, brake: %4d, throttle: %4d, misc: 0x%02x",
        //          myGamepad->dpad(),        // DPAD
        //          myGamepad->buttons(),     // bitmask of pressed buttons
        //          myGamepad->axisX(),       // (-511 - 512) left X Axis
        //          myGamepad->axisY(),       // (-511 - 512) left Y axis
        //          myGamepad->axisRX(),      // (-511 - 512) right X axis
        //          myGamepad->axisRY(),      // (-511 - 512) right Y axis
        //          myGamepad->brake(),       // (0 - 1023): brake button
        //          myGamepad->throttle(),    // (0 - 1023): throttle (AKA gas) button
        //          myGamepad->miscButtons()  // bitmak of pressed "misc" buttons
        // );

        // You can query the axis and other properties as well. See Gamepad.h
        // For all the available functions.
    }

    delay(150);
}
