#ifndef APP_DETECTION_HPP
#define APP_DETECTION_HPP

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


void test_detection(void);

class DlDetection {
public:
    const char *TAG = "DL_APP";
    struct RGBFrame {
        uint8_t *buf;
        int width;
        int height;
    };
    struct BBoxPos {
        int x_pos;
        int y_pos;
        int bb_width;
        int bb_height;
        int image_width;  // Width of the image
        int image_height; // Height of the image
    };
    uint8_t *rgb888_image_data;
    RGBFrame rgb888_pic;
    BBoxPos bbox;

    DlDetection(HumanFaceDetectMSR01 &s1, HumanFaceDetectMNP01 &s2);
    // DlDetection();
    ~DlDetection();

    void init();
    void init_rgb888();
    void startContinuousDetection();
    void runDetectionOnFrame(camera_fb_t *fb);
    RGBFrame* takePictureRGB888();
    void convertRGB565toRGB888(const uint8_t *src, uint8_t *dst, int width, int height);
    void detectFace(uint8_t *rgb888_image_data, int width, int height);
    void detectFaceJpeg(camera_fb_t *fb);
    void setBBox(BBoxPos);
    BBoxPos getBBbox();
    int get_bb_x();
    int get_bb_y();
    int get_bb_width();
    int get_bb_height();



private:

    void takePicture(uint8_t *buffer, int &height, int &width);

    HumanFaceDetectMSR01 *_s1;
    HumanFaceDetectMNP01 *_s2;
};

#endif // FACE_DETECTION_HPP