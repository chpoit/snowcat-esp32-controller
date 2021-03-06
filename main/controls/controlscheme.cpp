
#include "controlscheme.h"
#include <Arduino.h>
#include "stickdata.h"

ControlScheme::ControlScheme(GamepadPtr gamepad, int deadzone) {
    this->gamepad = gamepad;
    this->deadzone = deadzone;
}

void ControlScheme::calibrate(StickData stickData) {
    this->stickData = stickData;
}

void ControlScheme::setLEDs() {
    gamepad->setPlayerLEDs(playerLEDs);
    gamepad->setColorLED(std::get<0>(color), std::get<1>(color), std::get<2>(color));
}

double ControlScheme::scaleX(int axis) {
    if (axis > 0) {
        return axis * 511. / stickData.maxX;
    } else {
        return axis * -512. / stickData.minX;
    }
}

double ControlScheme::scaleY(int axis) {
    if (axis > 0) {
        return axis * 511. / stickData.maxY;
    } else {
        return axis * -512. / stickData.minY;
    }
}

double ControlScheme::scaleRX(int axis) {
    if (axis > 0) {
        return axis * 511. / stickData.maxRX;
    } else {
        return axis * -512. / stickData.minRY;
    }
}

double ControlScheme::scaleRY(int axis) {
    if (axis > 0) {
        return axis * 511. / stickData.maxRY;
    } else {
        return axis * -512. / stickData.minRY;
    }
}
