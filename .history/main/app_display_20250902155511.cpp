#define U8X8_USE_SPI
#undef U8X8_USE_I2C

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <string.h>
#include <u8g2.h>
#include "sdkconfig.h"
extern "C" {
#include "u8g2_esp32_hal.h"
}

#include "app_display.hpp"



tDisplay::tDisplay(){
    this->init();

}

tDisplay::~tDisplay() {
    // nothing to clean up for now
}

void tDisplay::init(){
  u8g2_esp32_hal.bus.spi.clk = PIN_CLK;
  u8g2_esp32_hal.bus.spi.mosi = PIN_MOSI;
  u8g2_esp32_hal.bus.spi.cs = PIN_CS;
  u8g2_esp32_hal.dc = PIN_DC;
  u8g2_esp32_hal.reset = PIN_RESET;
  u8g2_esp32_hal.bus.i2c.sda  = U8G2_ESP32_HAL_UNDEFINED;
  u8g2_esp32_hal.bus.i2c.scl  = U8G2_ESP32_HAL_UNDEFINED;
  ESP_LOGI(tag, "HAL init with pins: CLK=%d, MOSI=%d, CS=%d, DC=%d, RST=%d", 
           PIN_CLK, PIN_MOSI, PIN_CS, PIN_DC, PIN_RESET);
  
  u8g2_esp32_hal_init(u8g2_esp32_hal);
  
  ESP_LOGI(tag, "Setting up SSD1309 with Software SPI...");
  
  // void u8g2_Setup_ssd1309_128x64_noname0_f(u8g2_t *u8g2, const u8g2_cb_t *rotation, u8x8_msg_cb byte_cb, u8x8_msg_cb gpio_and_delay_cb)
  u8g2_Setup_ssd1309_128x64_noname0_f(&u8g2, U8G2_R0, u8g2_esp32_spi_byte_cb, u8g2_esp32_gpio_and_delay_cb);

  ESP_LOGI(tag, "Initializing display...");
  u8g2_InitDisplay(&u8g2);
  ESP_LOGI(tag, "Setting power save off...");
  u8g2_SetPowerSave(&u8g2, 0);
  
  ESP_LOGI(tag, "SSD1309 display initialized!");

}

void tDisplay::test_display(){
  int x_pos = 20;
  int y_pos = 20;

  // for(int i; i < 20; i++)
  while(1)
  {
    ESP_LOGI(tag, "X and Y position counters: %d %d", x_pos, y_pos);
    u8g2_ClearBuffer(&u8g2);
    
    // void u8g2_DrawCircle(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, uint8_t option)
    u8g2_DrawCircle(&u8g2, x_pos, y_pos, 10, U8G2_DRAW_ALL);
    u8g2_SendBuffer(&u8g2);

    x_pos = x_pos + 5;
    if (x_pos >= 100){ 
      x_pos = 20;
      y_pos = y_pos + 5;
      }
    if (y_pos >= 50) y_pos = 20;
    vTaskDelay(pdMS_TO_TICKS(20));

  }
}

void tDisplay::draw_circle_c(int x_pos, int y_pos, int rad){
  u8g2_ClearBuffer(&u8g2);
  u8g2_DrawCircle(&u8g2, x_pos, y_pos, rad, U8G2_DRAW_ALL);
  u8g2_SendBuffer(&u8g2);
}



void tDisplay::clear_screen(){
  u8g2_ClearBuffer(&u8g2);
}

void tDisplay::map_camera_to_display(int cam_x, int cam_y, int cam_width, int cam_height,
                                     int &disp_x, int &disp_y) {
    // Clamp input to camera frame
    if (cam_x < 0) cam_x = 0;
    if (cam_x >= cam_width) cam_x = cam_width - 1;
    if (cam_y < 0) cam_y = 0;
    if (cam_y >= cam_height) cam_y = cam_height - 1;

    // Map to display resolution
    disp_x = (cam_x * 128) / cam_width;  // scale X to 0..127
    disp_y = (cam_y * 64) / cam_height;  // scale Y to 0..63
}


void test_t_display(){
  
  tDisplay disp;
  disp.test_display();
}