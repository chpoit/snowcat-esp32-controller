
#include <Bluepad32.h>
#include "controlscheme.h"

#ifndef TANK_H
#define TANK_H

class Tank : public ControlScheme {
   public:
    Tank(GamepadPtr gamepad, int deadzone) : ControlScheme(gamepad, deadzone){};
    std::pair<int, int> handleThrottle(void);

   private:
    int getAxisValue(int axis, bool forward, bool backward, int throttleValue, int model);
};

#endif