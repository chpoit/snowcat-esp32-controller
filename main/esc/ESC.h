#include "ESP32Servo.h"
#include "scaler.h"

#ifndef ESC_H
#define ESC_H

class ESC {
   private:
    Servo servo;
    Scaler* scaler;
    int pin;
    int min;
    int max;
    int centerPoint;
    int centerOffset;

   public:
    ESC(int pin, int min, int max, int centerPoint, int centerOffset = 0);
    void arm(void);
    void disarm(void);
    void spin(int pwm);
    Scaler* getScaler(void);
};

#endif