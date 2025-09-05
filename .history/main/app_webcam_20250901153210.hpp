#ifndef APP_WEBCAM_HPP
#define APP_WEBCAM_HPP

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>
#include <esp_system.h>
#include <nvs_flash.h>
#include <sys/param.h>
#include <string.h>



#include "esp_camera.h"

#include <stdio.h>
#include "human_face_detect_msr01.hpp"
#include "human_face_detect_mnp01.hpp"
#include "dl_tool.hpp"
#include "dl_image.hpp"
#include "sensor.h"







class WebCam {
public:
    const char *TAG = "DL_APP";


    WebCam();
    ~WebCam();

    void init_wifi();
    void init_cam();
    void wifi_init_softap();
    void wifi_init_sta(); 
    void send_frame(uint8_t *buf, size_t len);
    void convertRGB565toRGB888(const uint8_t *src, uint8_t *dst, int width, int height);


private:



}



#endif // FACE_DETECTION_HPP