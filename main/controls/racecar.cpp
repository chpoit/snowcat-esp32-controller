#include "racecar.h"
#include <Arduino.h>

std::pair<double, double> Racecar::handleThrottle(void) {
    int forwards = 0;
    double left = 0.;
    double right = 0.;

    int backThrottle = gamepad->brake();
    int accelThrottle = gamepad->throttle();

    int xAxis = gamepad->axisRX();
    xAxis = abs(xAxis) < deadzone * 1.5 ? 0 : xAxis;

    // allows for "breaking" while going in a direction without letting the triggers go
    forwards = (accelThrottle - backThrottle) / 2;
    if (abs(forwards) < deadzone) {
        forwards = 0;
    }

    left = right = forwards;

    xAxis = scaleRX(xAxis);
    if (xAxis < 0) {
        left = left * max(1. - (xAxis / -511.), 0.);
    } else {
        right = right * max(1. - (xAxis / 512.), 0.);
    }

    return std::pair<double, double>(left, right);
}