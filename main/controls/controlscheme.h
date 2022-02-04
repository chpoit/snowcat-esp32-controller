#include <Bluepad32.h>
#include "stickdata.h"

#ifndef CONTROLSCHEME_H
#define CONTROLSCHEME_H

class ControlScheme {
   protected:
    GamepadPtr gamepad;
    StickData stickData;
    int deadzone;

   public:
    ControlScheme(GamepadPtr gamepad, int deadzone);
    virtual std::pair<int, int> handleThrottle(void) = 0;
    void calibrate(StickData stickData);
};

#endif