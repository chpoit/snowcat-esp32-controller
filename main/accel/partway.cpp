#include "partway.h"
#include <Arduino.h>

double Partway::accelerate(double targetPWM, double currentPWM) {
    double leftToGo = abs(targetPWM - currentPWM);

    double newChange = leftToGo > this->minChange ? leftToGo * this->scale : leftToGo;

    return (targetPWM > currentPWM ? currentPWM + newChange : currentPWM - newChange);
}