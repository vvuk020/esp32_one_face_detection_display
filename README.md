
# ESP32 Deep Learning Face Recognition with Transparent Display

This repository demonstrates how to use the ESP32 One kit (with the built in camera) in combination with a Deep Learning module for face recognition. The recognized face is displayed on a transparent display, with the position of the face (relative to the display) shown as a marked circle on the screen. For debugging purposes, there is an option to run a web server (pyserver.py) on a Python server to enable debugging functionalities.

Display is transparent display [1.51inch Transparent OLED](https://www.waveshare.com/wiki/1.51inch_Transparent_OLED) and the driver used is [SSD1309](https://files.waveshare.com/upload/9/9c/SSD1309Datasheet.pdf).

In this project, the ESP32 communicates with the display driver via SPI. I²C is not supported because the display library relies on deprecated I²C drivers that are incompatible with ESP-IDF v6.0.

## Requirements

* [ESP-IDF](https://github.com/espressif/esp-idf/tree/v6.0-dev) - IDF Version v6.0
* [ESP-DL](https://github.com/espressif/esp-dl/tree/idfv5.3) - DL Library for ESP32, Version idfv5.3
* [U8G2](https://github.com/olikraus/u8g2/tree/master) - For OLED Display
* [HAL for ESP32 for Display](https://github.com/mkfrey/u8g2-hal-esp-idf/tree/master) - Based on [u8g2-hal-esp-idf](https://github.com/mkfrey/u8g2-hal-esp-idf/tree/master) by mkfrey, but **modified** as the original didn't work as expected
* [ESP32-Camera (Version 2.1.0)](https://github.com/espressif/esp32-camera) - Camera is added using *idf.py add-dependency*

## Installation

1. **ESP-IDF Setup**:

   * Open the terminal created during the ESP-IDF installation.
   * Navigate to the main directory of the repository.

2. **Set Target Chip**:

   ```bash
   idf.py set-target esp32
   ```

3. **Build the Program**:

   ```bash
   idf.py build
   ```

4. **Upload the Program and Open the Serial Monitor**:

   ```bash
   idf.py flash monitor
   ```

### OLED Connection

The pins are connected as follows:

| *(ESP32)*  | *(OLED)*      |
| ---------- | ------------- |
| VCC        | 3V3           |
| GND        | GND           |
| CLK        | GPIO\_NUM\_21 |
| DIN (MOSI) | GPIO\_NUM\_22 |
| CS         | GPIO\_NUM\_26 |
| DC         | GPIO\_NUM\_0  |
| RESET      | GPIO\_NUM\_2  |

## Application

Once the ESP32 is powered on, the camera image is captured and displayed on the screen. The Deep Learning module performs face recognition, and a circle is displayed on the transparent display at the position of the recognized face.

### Webserver for Debugging

To debug the camera and the Deep Learning module, you need to call the `test_detection()` function in `main.cpp` and `pyserver.py`. The server runs in Python and processes the data captured by the camera and the Deep Learning module, as the ESP32 does not have enough memory to handle all processes.

The server will run on the URL displayed in the terminal and needs to be entered into the `app_detection.cpp` file to communicate with the ESP32. An example URL might look like `http://192.168.1.177:5000/`.

* In `main.cpp`, comment out the `test_detection()` function (and comment out all other functions in `main.cpp`).
* The web server is run in **Python** (pyserver.py). The server is used to transfer and process debugging data.
* The server URL is displayed in the terminal, for example: `http://192.168.1.177:5000/`. This URL must be entered into the `app_detection.cpp` file along with the Wi-Fi SSID and password parameters.

## Troubleshooting

* **M5 Flash Error**:
  If problems occur while flashing the ESP32 and an **M5 Flash Error** is shown, it is often caused by connected devices that draw power from the ESP32. To avoid this error, disconnect the **3V3 jumper** from the ESP32 pin and reconnect it after flashing is complete.

* **Changes and Issues with the Display HAL**:

  The ESP32 HAL for the display uses outdated I2C drivers, which conflict with the I2C drivers for the camera and sensors on the ESP32. Therefore, the file `u8g2_esp32_hal.c` has been modified to avoid initializing the I2C driver, as SPI is used for the display.

## Configuration

Changes to the ESP32 configuration can be made in the `sdkconfig.default` file (which corresponds to the configuration in `idf.py menuconfig`). If changes are made, the `sdkconfig` file must be deleted, and the program needs to be rebuilt to apply the new configuration.

### Configuration Parameters

All ESP32 configuration parameters are listed in the `sdkconfig.default` file.

---

### Notes

* If you encounter issues with memory size during flashing, check the partition table and make sure that the used flash memory is large enough (at least 8 MB recommended).

---

This translation should make the instructions clear for an English-speaking audience. If you need further adjustments or have questions, feel free to ask!
