#include "ESP32Servo.h"

#ifndef ESC_H
#define ESC_H

class ESC {
   private:
    Servo servo;
    int pin;

   public:
    ESC(int pin);
    void arm(void);
    void spin(int pwm);
};

#endif