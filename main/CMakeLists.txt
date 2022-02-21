set(srcs "main.c")

set(requires "bluepad32")

# See: https://gitlab.com/ricardoquesada/bluepad32/-/issues/9
if(DEFINED ENV{BLUEPAD32_ARDUINO})
    list(APPEND srcs "arduino_main.cpp")
    list(APPEND srcs "utils.cpp")
    list(APPEND srcs "accel/accel.cpp")
    list(APPEND srcs "accel/linear.cpp")
    list(APPEND srcs "accel/partway.cpp")
    list(APPEND srcs "accel/instant.cpp")
    list(APPEND srcs "esc/ESC.cpp")
    list(APPEND srcs "esc/scaler.cpp")
    list(APPEND srcs "controls/stickdata.cpp")
    list(APPEND srcs "controls/controlscheme.cpp")
    list(APPEND srcs "controls/tank.cpp")
    list(APPEND srcs "controls/racecar.cpp")
    list(APPEND requires "bluepad32_arduino" "arduino")
endif()

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "."
                    REQUIRES "${requires}" "ESP32Servo")
