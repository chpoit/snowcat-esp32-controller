#include "accel.h"

#ifndef INSTANT_H
#define INSTANT_H

class Instant : public Accel {
   private:
    int centerPoint;

   public:
    /* Instantly sets the new speed.
     * Scale is ignored.
     * if the direction is inverted, the motor will stop at the "centerPoint (should be 1500)" for 1 tick
     * It's probably uneeded, but w/e
     * Probably not the best acceleration to use.
     */
    Instant(double scale, int centerPoint) : Accel(scale) { this->centerPoint = centerPoint; };
    double accelerate(double targetPWM, double currentPWM);
};

#endif
