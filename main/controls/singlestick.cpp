#include "singlestick.h"
#include <Arduino.h>

std::pair<double, double> SingleStick::handleThrottle(void) {
    // todo: turning is a little too aggressive, but it should be fine for now
    double left = 0.;
    double right = 0.;
    double distance = 0.;

    int yAxis = gamepad->axisRY();
    int xAxis = gamepad->axisRX();
    yAxis = -scaleRY(abs(yAxis) < deadzone ? 0 : yAxis);
    xAxis = scaleRX(abs(xAxis) < deadzone ? 0 : xAxis);
    distance = sqrt(pow(yAxis, 2) + pow(xAxis, 2));


    if (yAxis < 0) {
        distance = -distance;
    }

    left = right = distance;

    if (xAxis < 0) {
        left = left * max(1. - (xAxis / -511.), 0.);
    } else {
        right = right * max(1. - (xAxis / 512.), 0.);
    }

    return std::pair<double, double>(left, right);
}
