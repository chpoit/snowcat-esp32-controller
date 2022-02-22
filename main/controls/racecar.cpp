#include "racecar.h"

std::pair<int, int> Racecar::handleThrottle(void) {
    int yAxis = 0;

    bool l2 = gamepad->l2();
    bool r2 = gamepad->r2();

    int backThrottle = gamepad->brake();
    int accelThrottle = gamepad->throttle();

    if (l2 && r2) {
        yAxis = 0;
    } else {
    }

    return std::pair<int, int>(0, 0);
}