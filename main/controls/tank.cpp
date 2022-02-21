
#include "tank.h"
#include "../utils.h"

std::pair<int, int> Tank::handleThrottle() {
    int model = gamepad->getModel();

    int leftSpeed = gamepad->axisY();
    int rightSpeed = gamepad->axisRY();

    int leftThrottle = gamepad->brake();
    int rightThrottle = gamepad->throttle();

    bool l1 = gamepad->l1();
    bool l2 = gamepad->l2();
    bool r1 = gamepad->r1();
    bool r2 = gamepad->r2();

    leftSpeed = getAxisValue(leftSpeed, l2, l1, leftThrottle, model);
    rightSpeed = getAxisValue(rightSpeed, r2, r1, rightThrottle, model);

    leftSpeed = scaleY(leftSpeed);
    rightSpeed = scaleRY(rightSpeed);

    return std::pair<int, int>(leftSpeed, rightSpeed);
}

int Tank::getAxisValue(int axis, bool forward, bool backward, int throttleValue, int model) {
    if (forward && backward) {
        return 0;
    }
    if (forward) {
        // Throttle reports as 0 for controllers without analog
        if (model == Gamepad::CONTROLLER_TYPE_WiiController || model == Gamepad::CONTROLLER_TYPE_SwitchProController ||
            model == Gamepad::CONTROLLER_TYPE_SwitchJoyConLeft || model == Gamepad::CONTROLLER_TYPE_SwitchJoyConRight ||
            model == Gamepad::CONTROLLER_TYPE_SwitchJoyConPair) {
            return 511;
        }
        return min(int(ceil(throttleValue / 2)), 511);
    }
    if (backward) {
        return -512;
    }
    // Use this to account for
    if (abs(axis) < deadzone) {
        return 0;
    }
    // I just want forward to be positive and backwards to be negative, so I invert the values here
    axis = -axis;
    return axis;
}
