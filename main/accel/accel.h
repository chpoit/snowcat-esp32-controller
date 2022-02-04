
#ifndef ACCEL_H
#define ACCEL_H

class Accel {
   public:
    Accel(double scale = 1);
    virtual double accelerate(double targetPWM, double currentPWM) = 0;

   protected:
    double scale = 1;
};

#endif
