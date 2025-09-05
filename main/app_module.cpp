#include "app_detection.hpp"
#include "app_display.hpp"
#include "app_webcam.hpp"
extern "C" {
#include "u8g2_esp32_hal.h"
}
#include "esp_camera.h"
// Your pin definitions for SSD1309
// #define PIN_CLK   GPIO_NUM_21     // pin 21 → I/O → safe
// #define PIN_MOSI  GPIO_NUM_22     // pin 22 → I/O → safe
// #define PIN_CS    GPIO_NUM_26     // pin 26 → I/O → safe
// #define PIN_DC    GPIO_NUM_0      // pin  0 → I/O → safe
// #define PIN_RESET GPIO_NUM_2      // pin  2 → I/O → safe


void test_module(){
    HumanFaceDetectMSR01 s1(0.1F, 0.5F, 10, 0.2F);
    HumanFaceDetectMNP01 s2(0.5F, 0.3F, 5);
    DlDetection detection(s1, s2);
    detection.init_rgb888();

    tDisplay disp;
    disp.clear_screen();
    // disp.test_display();

    int x_cal;
    int y_cal;
    int prev_x = 0;
    int prev_y = 0;
    int clear_flag = false;
    const char *tag_dl = "Disp_DL_test";
    while (1){
        DlDetection::RGBFrame* frame = detection.takePictureRGB888(); //detection->takePictureRGB888();
        ESP_LOGI(tag_dl, "RGB888 frame address: %p", (void*)frame->buf);
        ESP_LOGI(tag_dl, "RGB888 frame shape: [height: %d, width: %d, channels: 3]", frame->height, frame->width);

        detection.detectFace(frame->buf, frame->width, frame->height);
        disp.map_camera_to_display(detection.bbox.x_pos, detection.bbox.y_pos, frame->width, frame->height, x_cal, y_cal);
        ESP_LOGI(tag_dl, "bb.x_raw: %d; x_cal: %d; bb.y_raw: %d; y_raw: %d", detection.bbox.x_pos, detection.bbox.y_pos, x_cal, y_cal);

       
        if (detection.bbox.x_pos !=0 || detection.bbox.y_pos != 0){
            // disp.draw_circle_c(x_cal, y_cal, 20);
            disp.draw_number_circle(x_cal, y_cal, 15, 88);
            clear_flag = false;
        }
        else {clear_flag = true;}

        if (clear_flag){
            if (x_cal != prev_x || y_cal != prev_y) disp.clear_screen();
        }

        prev_x = x_cal;
        prev_y = y_cal;
        vTaskDelay(pdMS_TO_TICKS(100));
    
    }
}