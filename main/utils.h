#include <Arduino.h>
#include <Bluepad32.h>
#ifndef UTILS_H
#define UTILS_H

namespace utils {
int getSpeed(int axis,
             bool forward,
             bool backward,
             int throttleValue,
             int model,
             int DEADZONE,
             int calibratedMin,
             int calibratedMax);

void blinker(void* params);
void blinkController(GamepadPtr gp);
}  // namespace utils

#endif