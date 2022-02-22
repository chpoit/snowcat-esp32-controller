#include "racecar.h"
#include <Arduino.h>

std::pair<int, int> Racecar::handleThrottle(void) {
    int forwards = 0;
    int sideways = 0;
    double left = 0.;
    double right = 0.;

    bool l2 = gamepad->l2();
    bool r2 = gamepad->r2();

    int backThrottle = gamepad->brake();
    int accelThrottle = gamepad->throttle();

    int xAxis = gamepad->axisRX();
    xAxis = abs(xAxis) < deadzone ? 0 : xAxis;

    // l2/r2 check is there in case the throttle/brakes don't report 1023 as their max values.
    // deadzone should cover it, but you never know.
    forwards = (l2 && r2) ? 0 : (accelThrottle - backThrottle) / 2;
    if (forwards < deadzone) {
        forwards = 0;
    }

    left = right = forwards;

    xAxis = scaleRX(xAxis);
    if (xAxis < 0) {
        left = left * max(1. - (xAxis / -511.), 0.);
    } else {
        right = right * max(1. - (xAxis / 512.), 0.);
    }

    return std::pair<int, int>(left, right);
}