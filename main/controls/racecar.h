
#include <Bluepad32.h>
#include "controlscheme.h"

#ifndef RACECAR_H
#define RACECAR_H

class Racecar : public ControlScheme {
   public:
    Racecar(GamepadPtr gamepad, int deadzone) : ControlScheme(gamepad, deadzone) {
        playerLEDs = 2;
        color = std::tuple<int, int, int>(0, 255, 0);
    };
    std::pair<int, int> handleThrottle(void);
};

#endif