#include "ESP32Servo.h"

#ifndef ESC_H
#define ESC_H

class ESC {
   private:
    Servo servo;
    int pin;
    int min;
    int max;
    int centerPoint;

   public:
    ESC(int pin, int min, int max, int centerPoint);
    void arm(void);
    void disarm(void);
    void spin(int pwm);
};

#endif