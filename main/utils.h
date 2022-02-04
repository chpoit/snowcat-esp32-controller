#include <Arduino.h>
#include <Bluepad32.h>
#include "controls/stickdata.h"
#ifndef UTILS_H
#define UTILS_H

namespace utils {

void blinker(void* params);
void blinkController(GamepadPtr gp);
void calibrateSticks(GamepadPtr gp, StickData* stickData);

}  // namespace utils

#endif