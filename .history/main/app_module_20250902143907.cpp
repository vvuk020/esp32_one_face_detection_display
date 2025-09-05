#include "app_detection.hpp"
#include "app_display.hpp"


void test_module(){
    // tDisplay disp;

    HumanFaceDetectMSR01 s1(0.1F, 0.5F, 10, 0.2F);
    HumanFaceDetectMNP01 s2(0.5F, 0.3F, 5);
    DlDetection* detection = new DlDetection(s1, s2);
    detection->init_rgb888();

    const char *tag_dl = "TAKE_RGB888_TEST";

    int x_cal;
    int y_cal;
    while (1){
        DlDetection::RGBFrame* frame = detection->takePictureRGB888();
        ESP_LOGI(tag_dl, "RGB888 frame address: %p", (void*)frame->buf);
        ESP_LOGI(tag_dl, "RGB888 frame shape: [height: %d, width: %d, channels: 3]", frame->height, frame->width);

        detection->detectFace(frame->buf, frame->width, frame->height);
        // disp.map_camera_to_display(detection->bbox.x_pos, detection->bbox.y_pos
        //                             frame->width, frame->height, x_cal, y_cal);
        // disp.draw_circle_c(detection->bbox.x_pos, detection->bbox.x_pos, 20);

        vTaskDelay(pdMS_TO_TICKS(100)); // control FPS
    }
    delete detection;
}