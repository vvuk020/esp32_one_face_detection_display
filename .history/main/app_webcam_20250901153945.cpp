#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_camera.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_err.h"

#include "dl_image.hpp"
#include "app_webcam.hpp"


#define WIFI_SSID      "das_labor"
#define WIFI_PASS      "rosrosros"
#define SERVER_URL     "http://192.168.1.177:5000/upload"  // http://127.0.0.1:5000/upload"

static const char *TAG = "ESP32_CAM";