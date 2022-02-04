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

   public:
    ESC(int pin, int min, int max, int centerPoint);
    void arm(void);
    void disarm(void);
    void spin(int pwm);
    Scaler* getScaler(void);
};

#endif