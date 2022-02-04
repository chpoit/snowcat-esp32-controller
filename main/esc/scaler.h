
#ifndef SCALER_H
#define SCALER_H

class Scaler {
   public:
    Scaler(int escLowPoint, int escCenterPoint, int escHighPoint);
    double scaleAxis(int axisValue);

   private:
    int escLowPoint;
    int escCenterPoint;
    int escHighPoint;
    int lowRange;
    int highRange;
    double axisMin = -512.;
    double axisMax = 511.;
};

#endif