#include <Bluepad32.h>
#include "stickdata.h"

#ifndef CONTROLSCHEME_H
#define CONTROLSCHEME_H

class ControlScheme {
   protected:
    GamepadPtr gamepad;
    StickData stickData;
    int deadzone;

    double scaleX(int axis);
    double scaleY(int axis);
    double scaleRX(int axis);
    double scaleRY(int axis);

   public:
    ControlScheme(GamepadPtr gamepad, int deadzone);
    virtual std::pair<double, double> handleThrottle(void) = 0;
    void calibrate(StickData stickData);

    int playerLEDs = 0;
    std::tuple<int, int, int> color = {0, 0, 0};

    void setLEDs();
};

#endif