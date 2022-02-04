#include "ESC.h"

ESC::ESC(int pin) {
    this->pin = pin;
    this->servo = Servo();
}

void ESC::arm(void) {
    this->servo.attach(this->pin);
    this->servo.writeMicroseconds(1500);
}

void ESC::spin(int pwm) {
    this->servo.writeMicroseconds(pwm);
}