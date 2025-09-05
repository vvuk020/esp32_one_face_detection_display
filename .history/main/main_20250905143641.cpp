#include <stdio.h>
#include "app_webcam.hpp"
#include "app_detection.hpp"
#include "app_display.hpp"
#include "app_module.hpp"
#include "esp_log.h"

extern "C"{
void app_main(void)
{
    test_module(); // -> Whole module; Camera + DL + Display 
    // test_detection(); // For debugging and setting the camera on the webserver; Camera + DL + Server

    /*
    For testing purposes of webcame only and display only

    test_web_cam();
    test_t_display();
    */
}
}