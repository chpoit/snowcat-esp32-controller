#include <Bluepad32.h>

#ifndef controlscheme_H
#define controlscheme_H

class ControlScheme {
   protected:
    GamepadPtr gamepad;
    int deadzone;

   public:
    ControlScheme(GamepadPtr gamepad, int deadzone);
    virtual std::pair<int, int> handleThrottle(void) = 0;
    virtual void calibrate() = 0;
};

#endif