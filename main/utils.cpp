#include <Arduino.h>
#include <Bluepad32.h>

TaskHandle_t* blinkerTask = nullptr;

double scaleToESC(int axisValue, int ESC_LOW_POINT, int ESC_CENTER_POINT, int ESC_HIGH_POINT) {
    double lowRange = (ESC_CENTER_POINT - ESC_LOW_POINT);
    double highRange = (ESC_HIGH_POINT - ESC_CENTER_POINT);
    if (axisValue < 0) {
        auto scalar = axisValue / -512.;
        return ESC_CENTER_POINT - (scalar * lowRange);
    }
    if (axisValue > 0) {
        auto scalar = axisValue / 511.;
        return ESC_CENTER_POINT + (scalar * highRange);
    }

    return double(ESC_CENTER_POINT);
}

int getSpeed(int axis,
             bool forward,
             bool backward,
             int throttleValue,
             int model,
             int DEADZONE,
             int calibratedMin,
             int calibratedMax) {
    if (forward && backward) {
        return 0;
    }
    if (forward) {
        // Throttle reports as 0 for controllers without analog
        if (model == Gamepad::CONTROLLER_TYPE_WiiController || model == Gamepad::CONTROLLER_TYPE_SwitchProController ||
            model == Gamepad::CONTROLLER_TYPE_SwitchJoyConLeft || model == Gamepad::CONTROLLER_TYPE_SwitchJoyConRight ||
            model == Gamepad::CONTROLLER_TYPE_SwitchJoyConPair) {
            return 511;
        }
        return min(int(ceil(throttleValue / 2)), 511);
    }
    if (backward) {
        return -512;
    }
    // Use this to account for
    if (abs(axis) < DEADZONE) {
        return 0;
    }
    // I just want forward to be positive and backwards to be negative, so I invert the values here
    axis = -axis;
    if (axis > 0) {
        return axis * 511. / calibratedMax;
    } else {
        return axis * -512. / calibratedMin;
    }
}

double makeNice(double target, double current) {
    double leftToGo = abs(target - current);

    double newChange = min(1., leftToGo);

    return (target > current ? current + newChange : current - newChange);
}

void blinker(void* params) {
    auto gp = ((GamepadPtr)params);
    for (int i = 0; i < 10; i++) {
        int cLed = i % 2 ? 255 : 0;
        int pLed = i % 2 ? 1 : 0;

        gp->setColorLED(cLed, cLed, cLed);
        gp->setPlayerLEDs(pLed);
        delay(100);
    }
    vTaskDelete(NULL);
}

void blinkController(GamepadPtr gp) {
    if (blinkerTask != nullptr) {
        delete blinkerTask;
    }
    blinkerTask = new TaskHandle_t();
    xTaskCreatePinnedToCore(blinker,     /* Function to implement the task */
                            "blinker",   /* Name of the task */
                            10000,       /* Stack size in words */
                            (void*)gp,   /* Task input parameter */
                            0,           /* Priority of the task */
                            blinkerTask, /* Task handle. */
                            0);
}
