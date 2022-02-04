
#include "controlscheme.h"

ControlScheme::ControlScheme(GamepadPtr gamepad, int deadzone) {
    this->gamepad = gamepad;
    this->deadzone = deadzone;
}