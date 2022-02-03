# snowcat-esp32-controller

An esp32 based controller for the RCTF Snowcat

Project based on : https://gitlab.com/ricardoquesada/esp-idf-arduino-bluepad32-template

To connect a controller, just put it in sync mode and it should connect immediately.

The controls are "twinstick" controls, so the left side of the controller controls the left track, and the right side, the right track. 
- L2/R2 go forward (Analog controllers have variable speed)
- L1/R1 go backward
- Left Stick/Right stick both control one side
  - Only up/down movement is registered. left/right does nothing


# Building the firmware

Requires ESP-IDF **v4.4** or newer (5.0 seems to fail).

Includes the following ESP-IDF components, with a pre-configured `sdkconfig` file:

* [Arduino Core for ESP32](https://github.com/espressif/arduino-esp32) component
* [Bluepad32](https://gitlab.com/ricardoquesada/bluepad32/) component
* [BTStack](https://github.com/bluekitchen/btstack) component

## How to compile it

The following are the instructions for **Linux**, and should work for **macOS** as well. For **Windows**, use the
[ESP-IDF online installer][esp-idf-online-installer], and select `release/v4.4` branch.

1. Requirements and permissions

    Install ESP-IDF dependencies (taken from [here][toolchain-deps]):

    ```sh
    # For Ubuntu / Debian
    sudo apt-get install git wget flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
    ```

    And in case you don't have permissions to open `/dev/ttyUSB0`, do:
    (taken from [here][ttyusb0])

    ```sh
    # You MUST logout/login (or in some cases reboot Linux) after running this command
    sudo usermod -a -G dialout $USER
    ```

2. Install and setup ESP-IDF

    ```sh
    # Needs to be done just once
    # Clone the ESP-IDF git repo
    mkdir ~/esp && cd ~/esp
    git clone -b release/v4.4 --recursive https://github.com/espressif/esp-idf.git

    # Then install the toolchain
    cd ~/esp/esp-idf
    ./install.sh
    ```

3. Compile the template

    Clone the template:

    ```sh
    # Do it everytime you want to start a new project
    # Clone the template somewhere
    mkdir ~/src && cd ~/src
    git clone --recursive https://github.com/chpoit/snowcat-esp32-controller.git snowcat-esp32-controller
    ```

    Export the ESP-IDF environment variables in your shell:

    ```sh
    # Do it everytime you open a new shell
    # Optional: add it in your ~/.bashrc or ~/.profile
    source ~/esp/esp-idf/export.sh
    ```

    And finally compile and install your project.

    ```sh
    # Compile it
    cd <PATH-TO>/snowcat-esp32-controller
    idf.py build

    # Flash + open debug terminal
    idf.py flash
    ```
