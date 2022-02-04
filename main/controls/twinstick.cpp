
#include "twinstick.h"
#include "../utils.h"

TwinStick::TwinStick(GamepadPtr gamepad, int deadzone) : ControlScheme(gamepad, deadzone) {}

std::pair<int, int> TwinStick::handleThrottle() {
    auto gp = this->gamepad;

    int leftSpeed = gp->axisY();
    int rightSpeed = gp->axisRY();

    int leftThrottle = gp->brake();
    int rightThrottle = gp->throttle();

    bool l1 = gp->l1();
    bool l2 = gp->l2();
    bool r1 = gp->r1();
    bool r2 = gp->r2();

    leftSpeed = utils::getSpeed(leftSpeed, l2, l1, leftThrottle, gp->getModel(), this->deadzone, minXY, maxXY);
    rightSpeed = utils::getSpeed(rightSpeed, r2, r1, rightThrottle, gp->getModel(), this->deadzone, minRXY, maxRXY);

    return std::pair<int, int>(leftSpeed, rightSpeed);
}

void TwinStick::calibrate() {}