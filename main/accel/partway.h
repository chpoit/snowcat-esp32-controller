#include "accel.h"

#ifndef PARTWAY_H
#define PARTWAY_H

class Partway : public Accel {
   private:
    int minChange;

   public:
    /* The scale is how much of the "way to go" should be applied,
     * 0.5 scale will go half the way, until that value is lower than "minChange"
     * A value of 1 will be the same as "instant accel", minus the stop at 1500 pwm if the direction is inverted.
     */
    Partway(double scale, int minChange = 30) : Accel(scale) { this->minChange = minChange; };
    double accelerate(double targetPWM, double currentPWM);
};

#endif
