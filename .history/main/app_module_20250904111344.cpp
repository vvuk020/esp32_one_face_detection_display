#include "app_detection.hpp"
#include "app_display.hpp"
extern "C" {
#include "u8g2_esp32_hal.h"
}
#include "esp_camera.h"

// Your pin definitions for SSD1309
// #define PIN_CLK   GPIO_NUM_21     // pin 21 → I/O → safe
// #define PIN_MOSI  GPIO_NUM_22     // pin 22 → I/O → safe
// #define PIN_CS    GPIO_NUM_26     // pin 26 → I/O → safe
// #define PIN_DC    GPIO_NUM_0      //GPIO_NUM_25     // pin 25 → I/O → safe
// #define PIN_RESET GPIO_NUM_2      //GPIO_NUM_27     // pin 27 → I/O → safe



void test_module(){
    HumanFaceDetectMSR01 s1(0.1F, 0.5F, 10, 0.2F);
    HumanFaceDetectMNP01 s2(0.5F, 0.3F, 5);
    DlDetection detection(s1, s2);
    detection.init_rgb888();

    // esp_camera_deinit();

    tDisplay disp;
    // disp.test_display();

    while (1){
        DlDetection::RGBFrame* frame = detection.takePictureRGB888(); //detection->takePictureRGB888();
        ESP_LOGI(tag_dl, "RGB888 frame address: %p", (void*)frame->buf);
        ESP_LOGI(tag_dl, "RGB888 frame shape: [height: %d, width: %d, channels: 3]", frame->height, frame->width);

        detection->detectFace(frame->buf, frame->width, frame->height);
        detection.detectFace(frame->buf, frame->width, frame->height);
        disp.map_camera_to_display(detection->bbox.x_pos, detection->bbox.y_pos
                                    frame->width, frame->height, x_cal, y_cal);
        disp.draw_circle_c(detection.bbox.x_pos, detection.bbox.x_pos, 20);

        vTaskDelay(pdMS_TO_TICKS(100)); // control FPS
    }
    // delete detection;
}

void test_core_display(){
    // const char *tag_dl = "TAKE_RGB888_TEST";


    // ESP_LOGI(tag_dl, "Initializing HAL for Software SPI...");
  
    // u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
    // u8g2_esp32_hal.bus.spi.clk = PIN_CLK;
    // u8g2_esp32_hal.bus.spi.mosi = PIN_MOSI;
    // u8g2_esp32_hal.bus.spi.cs = PIN_CS;
    // u8g2_esp32_hal.dc = PIN_DC;
    // u8g2_esp32_hal.reset = PIN_RESET;
    
    // ESP_LOGI(tag_dl, "HAL init with pins: CLK=%d, MOSI=%d, CS=%d, DC=%d, RST=%d", 
    //         PIN_CLK, PIN_MOSI, PIN_CS, PIN_DC, PIN_RESET);
    
    // u8g2_esp32_hal_init(u8g2_esp32_hal);
    
    // u8g2_t u8g2;  
    // ESP_LOGI(tag_dl, "Setting up SSD1309 with Software SPI...");
    
    // // void u8g2_Setup_ssd1309_128x64_noname0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
    // u8g2_Setup_ssd1309_128x64_noname0_f(&u8g2, U8G2_R0, u8g2_esp32_spi_byte_cb, u8g2_esp32_gpio_and_delay_cb);

    // ESP_LOGI(tag_dl, "Initializing display...");
    // u8g2_InitDisplay(&u8g2);
    // ESP_LOGI(tag_dl, "Setting power save off...");
    // u8g2_SetPowerSave(&u8g2, 0);
    
    // ESP_LOGI(tag_dl, "SSD1309 display initialized!");
    // u8g2_DrawCircle(&u8g2, 20, 20, 20, U8G2_DRAW_ALL);

}