#include "instant.h"
#include <Arduino.h>

double Instant::accelerate(double targetPWM, double currentPWM) {
    int bigger = max(targetPWM, currentPWM);
    int smaller = min(targetPWM, currentPWM);

    if (smaller < this->centerPoint && this->centerPoint < bigger) {
        return this->centerPoint;
    }
    return targetPWM;
}