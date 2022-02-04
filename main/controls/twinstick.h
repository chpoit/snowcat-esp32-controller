
#include <Bluepad32.h>
#include "controlscheme.h"

#ifndef twinstick_H
#define twinstick_H

class TwinStick : public ControlScheme {
   public:
    TwinStick(GamepadPtr gamepad, int deadzone) : ControlScheme(gamepad, deadzone){};
    std::pair<int, int> handleThrottle(void);

   private:
    int getSpeed(int axis, bool forward, bool backward, int throttleValue, int model, int calibratedMin, int calibratedMax);
};

#endif