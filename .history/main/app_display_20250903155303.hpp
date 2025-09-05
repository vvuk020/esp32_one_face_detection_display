#ifndef APP_DISPLAY_HPP
#define APP_DISPLAY_HPP

#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <stdio.h>
#include <string.h>
#include <u8g2.h>
#include "sdkconfig.h"
#include "u8g2_esp32_hal.h"



// Your pin definitions for SSD1309
// #define PIN_CLK   GPIO_NUM_14     // SCK
// #define PIN_MOSI  GPIO_NUM_13     // MOSI/SDA  
// #define PIN_RESET GPIO_NUM_27     // RESET
// #define PIN_DC    GPIO_NUM_23     // DC
// #define PIN_CS    GPIO_NUM_18     // CS

#define PIN_CLK   GPIO_NUM_35
#define PIN_MOSI  GPIO_NUM_22
#define PIN_CS    GPIO_NUM_21
#define PIN_DC    GPIO_NUM_32
#define PIN_RESET GPIO_NUM_27

void test_t_display(void);


class tDisplay {
public:
  u8g2_esp32_hal_t u8g2_esp32_hal = U8G2_ESP32_HAL_DEFAULT;
  // u8g2_esp32_hal.bus.spi.clk = PIN_CLK;
  // u8g2_esp32_hal.bus.spi.mosi = PIN_MOSI;
  // u8g2_esp32_hal.bus.spi.cs = PIN_CS;
  // u8g2_esp32_hal.dc = PIN_DC;
  // u8g2_esp32_hal.reset = PIN_RESET;

 
  const char *tag = "t_Display";


  tDisplay();
  ~tDisplay();

  void init();
  void test_display();
  void draw_circle_c(int x_pos, int y_pos, int rad);
  void clear_screen();
  void map_camera_to_display(int cam_x, int cam_y, int cam_width, int cam_height,
                                int &disp_x, int &disp_y);
private:
  u8g2_t u8g2;  

};
#endif // APP_DISPLAY_HPP