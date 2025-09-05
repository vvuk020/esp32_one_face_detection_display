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

#include "app_detection.hpp"
#include "app_webcam.hpp"
#include "esp_heap_caps.h"
// #include "app_display.hpp"

#define WIFI_SSID      "das_labor"
#define WIFI_PASS      "rosrosros"
#define SERVER_URL     "http://192.168.1.177:5000/upload"  // http://127.0.0.1:5000/upload"
#define SERVER_BOX_URL "http://192.168.1.177:5000/box"


DlDetection::DlDetection(HumanFaceDetectMSR01 &s1, HumanFaceDetectMNP01 &s2):
    _s1(&s1), _s2(&s2)
    {
        this->init();
    }


DlDetection::~DlDetection() {
    if (rgb888_pic.buf) {
           free(rgb888_pic.buf);
           rgb888_pic.buf = nullptr;
       }
}

void DlDetection::init(){
    static camera_config_t camera_config;
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
    camera_config.frame_size = FRAMESIZE_QVGA;      //FRAMESIZE_QQVGA, FRAMESIZE_QVGA
    camera_config.jpeg_quality = 12;
    camera_config.fb_count = 1;
    camera_config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
    camera_config.fb_location = CAMERA_FB_IN_PSRAM;

    esp_err_t err = esp_camera_init(&camera_config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Camera init failed with error 0x%x", err);
        return;
    }

    sensor_t *sensor = esp_camera_sensor_get();
    if (sensor) {
        sensor->set_vflip(sensor, 1);
    }

}

void DlDetection::init_rgb888(){
    int width = 320;
    int height = 240;
    size_t size = width * height * 3;

    // rgb888_pic.buf = (uint8_t *)malloc(width*height*3);
    // rgb888_pic.buf = (uint8_t *)heap_caps_malloc(width * height * 3, MALLOC_CAP_IRAM);
    rgb888_pic.buf = (uint8_t *)heap_caps_malloc(width * height * 3, MALLOC_CAP_SPIRAM);
    if (!rgb888_pic.buf) {
        ESP_LOGE(TAG, "Failed to allocate RGB buffer");
        return;
    }
    ESP_LOGI(TAG, "RGB buffer size: %zu bytes, shape: [%d, %d, 3]", size, width, height);

}

void DlDetection::takePicture(uint8_t *buffer, int &height, int &width){
    camera_fb_t *pic = esp_camera_fb_get();
    
        if (!pic) {
            ESP_LOGE(TAG, "Camera capture failed");
            return;
        }
        memcpy(buffer, pic->buf, pic->len);
        height = pic->height;
        width = pic->width;

        
    ESP_LOGI(TAG, "Picture taken! Its size was: %zu bytes", pic->len);
    ESP_LOGI(TAG, "Image shape: [%d, %d, %d, %d]", height, width, 3, pic->format);

}


DlDetection::RGBFrame* DlDetection::takePictureRGB888(){
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb){
        ESP_LOGE(TAG, "Camera capture failed");
        // return rgb888_pic;
        return nullptr;
    }

    this->convertRGB565toRGB888(fb->buf, rgb888_pic.buf, fb->width, fb->height);

    // rgb888_pic.buf    = rgb888_image_data;
    rgb888_pic.width  = fb->width;
    rgb888_pic.height = fb->height;



    ESP_LOGI(TAG, "Picture taken! Its size was: %zu bytes", fb->len);
    ESP_LOGI(TAG, "Image shape: [%d, %d, %d, %d]", fb->height, fb->width, 3, fb->format);
    esp_camera_fb_return(fb);

    RGBFrame *ptr_rgb888 = &rgb888_pic;
    return ptr_rgb888;
}

void DlDetection::convertRGB565toRGB888(const uint8_t *src, uint8_t *dst, int width, int height) {
    const uint16_t *src16 = (const uint16_t *)src; // each pixel is 16 bits (RGB565)
    for (int i = 0; i < width * height; i++) {
        dl::image::convert_pixel_rgb565_to_rgb888(src16[i], &dst[i * 3]);
    }
    // ESP_LOGI(TAG, "Picture converted: [%d, %d, %d, %d]", dst, height, width, 3);
    ESP_LOGI(TAG, "Picture converted: [%d, %d, %d, %d]", dst, height, width, 3);
}


void DlDetection::detectFace(uint8_t *rgb888_image_data, int width, int height) {
    dl::tool::Latency latency;
    latency.start();

    // HumanFaceDetectMSR01 s1(0.1F, 0.5F, 10, 0.2F);
    // HumanFaceDetectMNP01 s2(0.5F, 0.3F, 5);

    // Assume s1.infer returns results as std::list<dl::detect::result_t>
    // auto candidates = s1.infer((uint8_t *)rgb888_image_data, {height, width, 3});
    // auto results = s2.infer((uint8_t *)rgb888_image_data, {height, width, 3}, candidates);
    auto candidates = _s1->infer((uint8_t *)rgb888_image_data, {height, width, 3});
    auto results = _s2->infer((uint8_t *)rgb888_image_data, {height, width, 3}, candidates);

    latency.end();
    latency.print("Inference latency");

    int bb_x_center = 0;
    int bb_y_center = 0;
    int bb_width    = 0;
    int bb_height   = 0;
    
    int i = 0;
    for (const auto &prediction : results) {
        printf("[%d] score: %f, box: [%d, %d, %d, %d]\n", i, prediction.score, prediction.box[0], prediction.box[1], prediction.box[2], prediction.box[3]);

        printf("    left eye: (%3d, %3d), ", prediction.keypoint[0], prediction.keypoint[1]);
        printf("right eye: (%3d, %3d)\n", prediction.keypoint[6], prediction.keypoint[7]);
        printf("    nose: (%3d, %3d)\n", prediction.keypoint[4], prediction.keypoint[5]);
        printf("    mouth left: (%3d, %3d), ", prediction.keypoint[2], prediction.keypoint[3]);
        printf("mouth right: (%3d, %3d)\n\n", prediction.keypoint[8], prediction.keypoint[9]);
        
        
        // Calculate the center and width/height of the bounding box
        bb_width = prediction.box[2] - prediction.box[0];
        bb_height = prediction.box[3] - prediction.box[1];
        bb_x_center = prediction.box[0] + bb_width / 2;
        bb_y_center = prediction.box[1] + bb_height / 2;
        
        i++;
    }

    BBoxPos bbox = {
        bb_x_center,
        bb_y_center,
        bb_width,
        bb_height,
        width,  // Image width
        height  // Image height
    };

    this->setBBox(bbox);
}

void DlDetection::setBBox(DlDetection::BBoxPos bbox_val){
    bbox = bbox_val;
     ESP_LOGI(TAG, "BBox Center: (%d, %d), Size: (%d x %d), Image Size: (%d x %d)", 
            bbox.x_pos, bbox.y_pos, 
            bbox.bb_width, bbox.bb_height,
            bbox.image_width, bbox.image_height);
}

DlDetection::BBoxPos DlDetection::getBBbox(){
    return bbox;
}

int DlDetection::get_bb_x(){
    return bbox.x_pos;
}

int DlDetection::get_bb_y(){
    return bbox.y_pos;
}

int DlDetection::get_bb_width(){
    return bbox.bb_width;
}

int DlDetection::get_bb_height(){
    return bbox.bb_height;
}

void test_detection_oop(){
    HumanFaceDetectMSR01 s1(0.1F, 0.5F, 10, 0.2F);
    HumanFaceDetectMNP01 s2(0.5F, 0.3F, 5);
    // DlDetection* detection = new DlDetection;
    DlDetection* detection = new DlDetection(s1, s2);


    const char *tag_dl = "DET_TEST";

    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb){
        ESP_LOGE(tag_dl, "Camera capture failed");
        return;
    }

    uint8_t *rgb888_image_data = (uint8_t *)malloc(fb->width * fb->height * 3);
    // uint8_t *rgb888_image_data = (uint8_t *)heap_caps_malloc(fb->width * fb->height * 3, MALLOC_CAP_SPIRAM);
    if (!rgb888_image_data) {
        ESP_LOGE(tag_dl, "Failed to allocate RGB buffer!");
        return;
    }

    detection->convertRGB565toRGB888(fb->buf, rgb888_image_data, fb->width, fb->height);
    ESP_LOGI(tag_dl, "RGB buffer at %p, size: %d x %d x 3", rgb888_image_data, fb->width, fb->height);
    detection->detectFace(rgb888_image_data, fb->width, fb->height);
    esp_camera_fb_return(fb);
    // delete detection;  // now you control when destructor runs

}




void test_detection(){
    HumanFaceDetectMSR01 s1(0.1F, 0.5F, 10, 0.2F);
    HumanFaceDetectMNP01 s2(0.5F, 0.3F, 5);
    DlDetection* detection = new DlDetection(s1, s2);
    detection->init_rgb888();

    WebCam web_cam(WIFI_SSID, WIFI_PASS, SERVER_URL);
    
    const char *tag_dl = "TAKE_RGB888_TEST";
    while (1){

        DlDetection::RGBFrame* frame = detection->takePictureRGB888();
        ESP_LOGI(tag_dl, "RGB888 frame address: %p", (void*)frame->buf);
        ESP_LOGI(tag_dl, "RGB888 frame shape: [height: %d, width: %d, channels: 3]", frame->height, frame->width);

        detection->detectFace(frame->buf, frame->width, frame->height);
        web_cam.send_frame(frame->buf, frame->width * frame->height * 3);
        web_cam.send_box(SERVER_BOX_URL, detection->bbox.x_pos, detection->bbox.y_pos);


        vTaskDelay(pdMS_TO_TICKS(100)); // control FPS
    }
    delete detection;

}
