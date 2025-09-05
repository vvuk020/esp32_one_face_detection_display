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

    WebCam(const std::string& wifi_ssid, const std::string& wifi_pass, const std::string& server_url);
    ~WebCam();

    void init_wifi();
    void init_cam();
    void wifi_init_softap();
    void wifi_init_sta(); 
    void send_frame(uint8_t *buf, size_t len);
    void convertRGB565toRGB888(const uint8_t *src, uint8_t *dst, int width, int height);


private:


    std::string wifi_ssid;
    std::string wifi_pass;
    std::string server_url;

}



#endif