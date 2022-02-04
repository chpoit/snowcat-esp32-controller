#include "scaler.h"

Scaler::Scaler(int escLowPoint, int escCenterPoint, int escHighPoint) {
    this->escLowPoint = escLowPoint;
    this->escCenterPoint = escCenterPoint;
    this->escHighPoint = escHighPoint;
    this->lowRange = escCenterPoint - escLowPoint;
    this->highRange = escHighPoint - escCenterPoint;
}

double Scaler::scaleAxis(int axisValue) {
    if (axisValue < 0) {
        auto scalar = axisValue / this->axisMin;
        return this->escCenterPoint - (scalar * lowRange);
    }
    if (axisValue > 0) {
        auto scalar = axisValue / this->axisMax;
        return this->escCenterPoint + (scalar * highRange);
    }

    return double(this->escCenterPoint);
}
