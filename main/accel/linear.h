#include "accel.h"

#ifndef LINEAR_H
#define LINEAR_H

class Linear : public Accel {
   public:
    Linear(double scale = 1) : Accel(scale){};
    double accelerate(double targetPWM, double currentPWM);
};

#endif
