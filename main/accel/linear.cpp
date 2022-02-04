#include "linear.h"
#include <Arduino.h>

double Linear::accelerate(double targetPWM, double currentPWM) {
    double leftToGo = abs(targetPWM - currentPWM);

    double newChange = min(this->scale, leftToGo);

    return (targetPWM > currentPWM ? currentPWM + newChange : currentPWM - newChange);
}