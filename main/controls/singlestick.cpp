#include "singlestick.h"
#include <Arduino.h>

std::pair<double, double> SingleStick::handleThrottle(void) {
    // todo: add edge of circle distance detection, otherwise it slows down while turning
    double left = 0.;
    double right = 0.;
    double forwards = 0.;

    int yAxis = gamepad->axisRY();
    int xAxis = gamepad->axisRX();
    yAxis = -scaleRY(abs(yAxis) < deadzone ? 0 : yAxis);
    xAxis = scaleRX(abs(xAxis) < deadzone ? 0 : xAxis);

    forwards = yAxis + xAxis;
    if (xAxis > 0 && yAxis > 0) {
        forwards = yAxis + xAxis;
        left = forwards;
        right = forwards * max(1. - (xAxis / 512.), 0.);
    } else if (xAxis > 0 && yAxis < 0) {
        left = forwards;
        right = forwards * max(1. - (xAxis / 512.), 0.);
    } else if (xAxis < 0 && yAxis > 0) {
        forwards = yAxis + xAxis;
        left = forwards * max(1. - (xAxis / -511.), 0.);
        right = forwards;
    } else if (xAxis < 0 && yAxis < 0) {
        left = forwards * max(1. - (xAxis / -511.), 0.);
        right = forwards;
    }

    // if (xAxis > 0) {
    //     // right -= abs(xAxis);
    //     left += xAxis;
    //     right = right * max(1. - (xAxis / 512.), 0.);
    // } else {
    //     right += xAxis;
    //     // left -= abs(xAxis);
    //     left = left * max(1. - (xAxis / -511.), 0.);
    // }

    return std::pair<double, double>(left, right);
}
