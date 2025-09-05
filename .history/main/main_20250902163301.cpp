#include <stdio.h>
#include "app_webcam.hpp"
#include "app_detection.hpp"
#include "app_display.hpp"
#include "app_module.hpp"
#include "esp_log.h"







extern "C"{
void app_main(void)
{
    // test_web_cam();
    // test_detection();
    // test_t_display();

    
    esp_log_level_set("*", ESP_LOG_VERBOSE);  // Enable all logs temporarily
    ESP_LOGI("MAIN", "Starting app_main...");
    test_module();


}
}