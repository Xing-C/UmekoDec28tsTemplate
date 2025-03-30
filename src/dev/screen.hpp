#ifndef _XLCD_SCREEN
#define _XLCD_SCREEN

#include <TFT_eSPI.h>
#include <lvgl.h>

#define SCREEN_BL_PIN 4
#define SCREEN_VDD 5
#define SCREEN_ROTATION 1
#define ROTATE 1
static const uint16_t screenWidth  = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];
int brightness = 100;  // 屏幕亮度

TFT_eSPI tft = TFT_eSPI(screenHeight, screenWidth); /* TFT instance */

// 平滑的亮起
void smooth_on(){
    analogWrite(SCREEN_BL_PIN, 0);
    analogWriteFreq(10000);
    for(int i=0; i<brightness; i++){
       analogWrite(SCREEN_BL_PIN, i);
       delay(2);
    }
    Serial.printf("[%6lu][Display] Smooth on\n", millis());
 }
 

 void my_disp_flush(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p){
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );
    tft.startWrite();
    tft.setAddrWindow( area->x1, area->y1, w, h );
    tft.pushColors( ( uint16_t * )&color_p->full, w * h, true );
    tft.endWrite();
    lv_disp_flush_ready( disp_drv );
 }

 // 平滑的熄灭
void smooth_off(){
    analogWrite(SCREEN_BL_PIN, brightness);
    analogWriteFreq(10000);
    for(int i=brightness; i>=0; i--){
       analogWrite(SCREEN_BL_PIN, i);
       delay(2);
    }
    Serial.printf("[%6lu][Display] Smooth off\n", millis());
 }

// 背光调节,会限制输入亮度在正确范围内
void set_brightness(int _brightness){
    if (_brightness < 255 && _brightness > 5){
       analogWriteFreq(10000);
       analogWrite(SCREEN_BL_PIN, _brightness);
       brightness = _brightness;
    }else if(_brightness >= 255){analogWrite(SCREEN_BL_PIN, 255); brightness=255;
    }else if(_brightness <= 5)   {analogWrite(SCREEN_BL_PIN, 5); brightness=5;
    }
 }

 void log_heap_usage(){
    Serial.printf("[%6lu][Memory] Free Heap: %d, Used Heap: %d, Total Heap: %d\n", millis(), rp2040.getFreeHeap(), rp2040.getUsedHeap(), rp2040.getTotalHeap());
 }
 
// 开启屏幕
void screen_setup(){
    pinMode(SCREEN_BL_PIN, OUTPUT);
    digitalWrite(SCREEN_BL_PIN, LOW);
    pinMode(SCREEN_VDD, OUTPUT);
    digitalWrite(SCREEN_VDD, LOW);
    lv_init();
    Serial.printf("[%6lu]", millis());
    Serial.println("[Display] Screen powerd on okay");
    tft.init();
    tft.setRotation( ROTATE ); /* Landscape orientation, flipped */
    tft.setSwapBytes(true);
    tft.initDMA();

    Serial.printf("[%6lu]", millis());
    Serial.println("[Display] TFT initialized");
 
    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );
    Serial.printf("[%6lu]", millis());
    Serial.println("[Display] lv disp draw buf init");
     // /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    Serial.printf("[%6lu]", millis());
    Serial.println("[Display] lv disp drv init");
    // /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    log_heap_usage();
    lv_disp_drv_register( &disp_drv );
    Serial.printf("[%6lu]", millis());
    Serial.println("[Display] lvgl initialized");

 }
 

#endif