
#include "controlscheme.h"
#include "stickdata.h"

ControlScheme::ControlScheme(GamepadPtr gamepad, int deadzone) {
    this->gamepad = gamepad;
    this->deadzone = deadzone;
}

void ControlScheme::calibrate(StickData stickData) {
    this->stickData = stickData;
}