#ifndef APP_WEBCAM_HPP
#define APP_WEBCAM_HPP

#include <stdio.h>
#include "esp_camera.h"
#include "sensor.h"





void test_web_cam(void);


class WebCam {
public:
    const char *TAG = "WEB_CAM";
    camera_config_t camera_config;
    uint8_t *rgb888_image_data;

    WebCam(const char *wifi_ssid, const char *wifi_pass, const char *server_url);
    ~WebCam();

    void init_wifi();
    esp_err_t init_cam();
    void wifi_init_softap();
    void wifi_init_sta(); 
    void send_frame(uint8_t *buf, size_t len);
    void convertRGB565toRGB888(const uint8_t *src, uint8_t *dst, int width, int height);


private:


    const char *wifi_ssid;
    const char *wifi_pass;
    const char *server_url;

};



#endif