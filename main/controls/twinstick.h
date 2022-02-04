
#include <Bluepad32.h>
#include "controlscheme.h"

#ifndef twinstick_H
#define twinstick_H

class TwinStick : public ControlScheme {
   public:
    TwinStick(GamepadPtr gamepad, int deadzone);
    std::pair<int, int> handleThrottle(void);
    void calibrate();

   private:
    int maxXY = 511;
    int maxRXY = 511;
    int minXY = -512;
    int minRXY = -512;
};

#endif