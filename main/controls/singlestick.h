
#include <Bluepad32.h>
#include "controlscheme.h"

#ifndef SINGLESTICK_H
#define SINGLESTICK_H

class SingleStick : public ControlScheme {
   public:
    SingleStick(GamepadPtr gamepad, int deadzone) : ControlScheme(gamepad, deadzone) {
        playerLEDs = 3;
        color = std::tuple<int, int, int>(0, 0, 255);
    };
    std::pair<int, int> handleThrottle(void);
};

#endif