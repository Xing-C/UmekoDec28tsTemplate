#include <Arduino.h>
#include "src/dev/screen.hpp"
#include "src/dev/touch.hpp"
#include <demos/lv_demos.h>


void setup() {
    // 在此处设置引脚模式、初始化串口通信等
    Serial.begin(115200); // 初始化串口通信
    delay(2000); // 等待串口稳定
    screen_setup();
    touch_init();
    lv_demo_widgets(); 
    lv_timer_handler(); /* let the GUI do its work */  
    delay(100);
    smooth_on();
  }
  
  // 主循环函数，程序运行时会反复执行
  void loop() {
    // 在此处编写主逻辑代码
    lv_timer_handler(); /* let the GUI do its work */
    delay( 5 );
  }


  void setup1() {
    // 多核代码
  }
  
  void loop1() {
     // 多核代码
  }