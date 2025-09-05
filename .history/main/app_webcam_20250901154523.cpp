#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_err.h"


#include "esp_camera.h"
#include "sensor.h"
#include "dl_image.hpp"

#include "app_webcam.hpp"


#define WIFI_SSID      "das_labor"
#define WIFI_PASS      "rosrosros"
#define SERVER_URL     "http://192.168.1.177:5000/upload"  // http://127.0.0.1:5000/upload"


WebCam::WebCam(const std::string& wifi_ssid, const std::string& wifi_pass, const std::string& server_url){

}


void WebCam::init_cam(){
    camera_config.pin_pwdn = -1;
    camera_config.pin_reset = -1;
    camera_config.pin_xclk = 4;
    camera_config.pin_sccb_sda = 18;
    camera_config.pin_sccb_scl = 23;

    camera_config.pin_d7 = 36;
    camera_config.pin_d6 = 37;
    camera_config.pin_d5 = 38;
    camera_config.pin_d4 = 39;
    camera_config.pin_d3 = 35;
    camera_config.pin_d2 = 14;
    camera_config.pin_d1 = 13;
    camera_config.pin_d0 = 34;

    camera_config.pin_vsync = 5;
    camera_config.pin_href = 27;
    camera_config.pin_pclk = 25;

    camera_config.xclk_freq_hz = 20000000;
    camera_config.ledc_timer = LEDC_TIMER_0;
    camera_config.ledc_channel = LEDC_CHANNEL_0;

    camera_config.pixel_format = PIXFORMAT_RGB565;  // PIXFORMAT_JPEG,
    camera_config.frame_size = FRAMESIZE_QQVGA;  //FRAMESIZE_QQVGA, FRAMESIZE_QVGA
    camera_config.jpeg_quality = 12;
    camera_config.fb_count = 1;
    camera_config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    camera_config.fb_location = CAMERA_FB_IN_PSRAM;


    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed with error 0x%x", err);
    }
    return err;
}

void WebCam::wifi_init_sta