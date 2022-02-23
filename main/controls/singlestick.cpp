#include "singlestick.h"
#include <Arduino.h>

std::pair<double, double> SingleStick::handleThrottle(void) {
    //todo: add edge of circle distance detection, otherwise it slows down while turning
    double left = 0.;
    double right = 0.;

    int yAxis = -gamepad->axisRY();
    int xAxis = gamepad->axisRX();
    yAxis = scaleRY(abs(yAxis) < deadzone ? 0 : yAxis);
    xAxis = abs(xAxis) < deadzone * 1.5 ? 0 : xAxis;

    left = right = yAxis;

    xAxis = scaleRX(xAxis);
    if (xAxis < 0) {
        left = left * max(1. - (xAxis / -511.), 0.);
    } else {
        right = right * max(1. - (xAxis / 512.), 0.);
    }

    return std::pair<double, double>(left, right);
}
