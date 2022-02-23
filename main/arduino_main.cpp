#include "sdkconfig.h"
#ifndef CONFIG_BLUEPAD32_PLATFORM_ARDUINO
#error "Must only be compiled when using Bluepad32 Arduino platform"
#endif  // !CONFIG_BLUEPAD32_PLATFORM_ARDUINO

#include <Arduino.h>
#include <Bluepad32.h>
#include "accel/accel.h"
#include "accel/instant.h"
#include "accel/linear.h"
#include "accel/partway.h"
#include "controls/controlscheme.h"
#include "controls/racecar.h"
#include "controls/singlestick.h"
#include "controls/stickdata.h"
#include "controls/tank.h"
#include "esc/ESC.h"
#include "utils.h"

#define LINEAR_ACCELERATION 1
#define PARTWAY_ACCELERATION 0.5
#define DEADZONE 50
#define LEFTESC_WHITE_PIN 2
#define RIGHTESC_WHITE_PIN 4

#define THROTTLE_MS_DELAY 10
#define ESC_CENTER_OFFSET -10

#define ESC_CENTER_POINT 1500
#define ESC_LOW_POINT 1100
#define ESC_HIGH_POINT 1900

// #define ESC_LOW_POINT (ESC_CENTER_POINT - 100)
// #define ESC_HIGH_POINT (ESC_CENTER_POINT + 100)

ESC LEFT_ESC = ESC(LEFTESC_WHITE_PIN, ESC_LOW_POINT, ESC_HIGH_POINT, ESC_CENTER_POINT, ESC_CENTER_OFFSET);
ESC RIGHT_ESC = ESC(RIGHTESC_WHITE_PIN, ESC_LOW_POINT, ESC_HIGH_POINT, ESC_CENTER_POINT, ESC_CENTER_OFFSET);

static GamepadPtr myGamepad;
static ControlScheme* controlScheme;
static utils::LinkedList<ControlScheme> schemeList;

// static Accel* acceleration = new Linear(LINEAR_ACCELERATION);
static Accel* acceleration = new Partway(PARTWAY_ACCELERATION);
StickData calibrationData = StickData();

int prevLeft = ESC_CENTER_POINT;
int prevRight = ESC_CENTER_POINT;

bool isCalibrating = false;

TaskHandle_t* controllerTask = nullptr;

void handleThrottle(void* params) {
    for (;;) {
        if (isCalibrating) {
            LEFT_ESC.spin(prevLeft = ESC_CENTER_POINT);
            RIGHT_ESC.spin(prevRight = ESC_CENTER_POINT);
            continue;
        }
        if (myGamepad && controlScheme && myGamepad->isConnected()) {
            auto axis = controlScheme->handleThrottle();
            double leftSpeed = axis.first;
            double rightSpeed = axis.second;

            double leftPWM = LEFT_ESC.getScaler()->scaleAxis(leftSpeed);
            double rightPWM = RIGHT_ESC.getScaler()->scaleAxis(rightSpeed);

            double leftLinearPWM = acceleration->accelerate(leftPWM, prevLeft);
            double rightLinearPWM = acceleration->accelerate(rightPWM, prevRight);

            Serial.println("ESCL\t" + String(leftLinearPWM) + "(" + leftPWM + ")" + "\tESCR\t" + String(rightLinearPWM) + "(" + rightPWM +
                           ")");

            LEFT_ESC.spin(leftLinearPWM);
            RIGHT_ESC.spin(rightLinearPWM);

            prevLeft = leftLinearPWM;
            prevRight = rightLinearPWM;
        }
        delay(THROTTLE_MS_DELAY);
    }
}

void createControllerTask(GamepadPtr gp) {
    if (controllerTask != nullptr) {
        delete controllerTask;
    }
    controllerTask = new TaskHandle_t();
    xTaskCreatePinnedToCore(handleThrottle, /* Function to implement the task */
                            "throttle",     /* Name of the task */
                            10000,          /* Stack size in words */
                            NULL,           /* Task input parameter */
                            0,              /* Priority of the task */
                            controllerTask, /* Task handle. */
                            0);
}

void onConnectedGamepad(GamepadPtr gp) {
    if (myGamepad != nullptr) {
        return;
    }
    createControllerTask(gp);

    myGamepad = gp;
    schemeList = {new Tank(myGamepad, DEADZONE), new Racecar(myGamepad, DEADZONE), new SingleStick(myGamepad, DEADZONE)};

    controlScheme = schemeList.getCurrent();
    controlScheme->calibrate(calibrationData);
    Serial.println("CALLBACK: CALIBRATED");
    controlScheme->setLEDs();
    Serial.println("CALLBACK: LEDS CONNECTED");

    Serial.println("CALLBACK: Gamepad is connected!");
    LEFT_ESC.arm();
    RIGHT_ESC.arm();
}

void onDisconnectedGamepad(GamepadPtr gp) {
    Serial.println("CALLBACK: Gamepad is disconnected!");
    myGamepad = nullptr;
    LEFT_ESC.disarm();
    RIGHT_ESC.disarm();

    vTaskDelete(*controllerTask);
}

void setup() {
    Serial.begin(115200);
    BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
    pinMode(LEFTESC_WHITE_PIN, OUTPUT);
    pinMode(RIGHTESC_WHITE_PIN, OUTPUT);
    delay(250);
}

void loop() {
    BP32.update();
    if (myGamepad && myGamepad->isConnected()) {
        if (!isCalibrating) {
            if (myGamepad->a() && myGamepad->b()) {
                utils::blinkController(myGamepad);
                isCalibrating = true;
                calibrationData = StickData::Zeroed();
                myGamepad->setColorLED(255, 255, 255);
                myGamepad->setPlayerLEDs(16);
            }
            if (myGamepad->miscBack()) {
                controlScheme = schemeList.getNext();
                controlScheme->calibrate(calibrationData);
                controlScheme->setLEDs();
            }
        } else {
            utils::calibrateSticks(myGamepad, &calibrationData);
            if (myGamepad->x() && myGamepad->y()) {
                // utils::blinkController(myGamepad);
                Serial.println("newVals: ");
                Serial.println("minY: " + String(calibrationData.minY));
                Serial.println("maxY: " + String(calibrationData.maxY));
                Serial.println("minX: " + String(calibrationData.minX));
                Serial.println("maxX: " + String(calibrationData.maxX));
                Serial.println("minRY: " + String(calibrationData.minRY));
                Serial.println("maxRY: " + String(calibrationData.maxRY));
                Serial.println("minRX: " + String(calibrationData.minRX));
                Serial.println("maxRX: " + String(calibrationData.maxRX));
                controlScheme->calibrate(calibrationData);
                controlScheme->setLEDs();

                isCalibrating = false;
            }
        }
    }
    delay(150);
}
