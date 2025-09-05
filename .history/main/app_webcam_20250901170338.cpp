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


WebCam::WebCam(const char *wifi_ssid, const char *wifi_pass, const char *server_url)
    : wifi_ssid(wifi_ssid), wifi_pass(wifi_pass), server_url(server_url) {
    
    this->init_wifi();
    this->rgb888_image_data = NULL;
}


WebCam::~WebCam() {
    // Clean up, if necessary
}

void WebCam::init_wifi(){
    ESP_ERROR_CHECK(nvs_flash_init());
    wifi_init_sta();

}

esp_err_t WebCam::init_cam(){
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

void WebCam::wifi_init_sta(){
    {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    wifi_config_t wifi_config = {};
    strncpy((char *)wifi_config.sta.ssid, wifi_ssid, sizeof(wifi_config.sta.ssid));
    strncpy((char *)wifi_config.sta.password, wifi_pass, sizeof(wifi_config.sta.password));

    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Connecting to Wi-Fi...");
    ESP_ERROR_CHECK(esp_wifi_connect());
}
}

void WebCam::convertRGB565toRGB888(const uint8_t *src, uint8_t *dst, int width, int height){
    const uint16_t *src16 = (const uint16_t *)src; // each pixel is 16 bits (RGB565)
    for (int i = 0; i < width * height; i++) {
        dl::image::convert_pixel_rgb565_to_rgb888(src16[i], &dst[i * 3]);
    }
    ESP_LOGI(TAG, "Picture converted: [%d, %d, %d, %d]", dst, height, width, 3);
}

void WebCam::send_frame(uint8_t *buf, size_t len)
{
    esp_http_client_config_t config = {0};

    config.url = server_url;
    config.method = HTTP_METHOD_POST;

    esp_http_client_handle_t client = esp_http_client_init(&config);

    esp_http_client_set_header(client, "Content-Type", "application/octet-stream");
    esp_http_client_set_post_field(client, (const char *)buf, len);

    esp_err_t err = esp_http_client_perform(client);


    if (err == ESP_OK) {
        ESP_LOGI(TAG, "Frame sent: HTTP %d",
                 esp_http_client_get_status_code(client));
    } else {
        ESP_LOGE(TAG, "HTTP request failed: %s", esp_err_to_name(err));
    }

    ESP_LOGI(TAG, "Sending frame of size: %zu from address: %p", len, buf);
    esp_http_client_cleanup(client);
}



void test_web_cam(){
    WebCam web_cam(WIFI_SSID, WIFI_PASS, SERVER_URL);
    ESP_ERROR_CHECK(web_cam.init_cam());
    const char *tag_wbc = "test_web_cam";

    while (1) {
        camera_fb_t *fb = esp_camera_fb_get();
        if (!fb) {
            ESP_LOGE(tag_wbc, "Camera capture failed");
            vTaskDelay(pdMS_TO_TICKS(200));
            continue;
        }

           if (web_cam.rgb888_image_data == NULL) {
               web_cam.rgb888_image_data = (uint8_t *)malloc(fb->width * fb->height * 3);
               if (!web_cam.rgb888_image_data) {
                   ESP_LOGE(tag_wbc, "Failed to allocate memory for RGB888 image");
                   break;
               }
           }

        web_cam.convertRGB565toRGB888(fb->buf, web_cam.rgb888_image_data, fb->width, fb->height);

        web_cam.send_frame(web_cam.rgb888_image_data, fb->width * fb->height * 3);

        esp_camera_fb_return(fb);


        vTaskDelay(pdMS_TO_TICKS(100)); // control FPS
    }
}



