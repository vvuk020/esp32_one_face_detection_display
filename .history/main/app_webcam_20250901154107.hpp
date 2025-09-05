#ifndef APP_WEBCAM_HPP
#define APP_WEBCAM_HPP

#include <stdio.h>
#include "esp_camera.h"
#include "sensor.h"

#define WIFI_SSID      "das_labor"
#define WIFI_PASS      "rosrosros"
#define SERVER_URL     "http://192.168.1.177:5000/upload" 

class WebCam {
public:
    const char *TAG = "WEB_CAM";
    camera_config_t camera_config;

    WebCam(const char wifi_ssid, const char wifi_pass, const char server_url);
    ~WebCam(const char wifi_ssid, const char wifi_pass, const char server_url);

    void init_wifi();
    void init_cam();
    void wifi_init_softap();
    void wifi_init_sta(); 
    void send_frame(uint8_t *buf, size_t len);
    void convertRGB565toRGB888(const uint8_t *src, uint8_t *dst, int width, int height);


private:


}



#endif // FACE_DETECTION_HPP