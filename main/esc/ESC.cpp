#include "ESC.h"
#include "scaler.h"

ESC::ESC(int pin, int min, int max, int centerPoint) {
    this->pin = pin;
    this->min = min;
    this->max = max;
    this->centerPoint = centerPoint;
    this->servo = Servo();
    this->scaler = new Scaler(min, centerPoint, max);
}

void ESC::arm(void) {
    this->servo.attach(this->pin);
    this->servo.writeMicroseconds(this->centerPoint);
}

void ESC::disarm(void) {
    this->servo.writeMicroseconds(this->centerPoint);
    this->servo.detach();
}

void ESC::spin(int pwm) {
    if (pwm > this->max) {
        pwm = this->max;
    }
    if (pwm < this->min) {
        pwm = this->min;
    }
    this->servo.writeMicroseconds(pwm);
}

Scaler* ESC::getScaler(void) {
    return this->scaler;
    // return 0;
}