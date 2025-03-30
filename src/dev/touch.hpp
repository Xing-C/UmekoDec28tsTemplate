#ifndef _CST816T_H
#define _CST816T_H
#include "FT6336U.h"
#include <lvgl.h>


#define TOUCH_SDA 2
#define TOUCH_SCL 3

#define TOUCH_RST PinNotUsed
#define ROTATE 1
#define TOUCH_LONG_PUSH_T 200

FT6336U touch_drv(TOUCH_SDA, TOUCH_SCL, TOUCH_RST, -1); 


void gesture_handler(uint8_t gesture){
    static uint8_t last_gesture = Gestures::None;
    if (last_gesture != gesture){
        if (gesture == Gestures::SlideUp){ 
            // TODO
        }else if (gesture == Gestures::SlideDown){
            // TODO
        }
        else if(gesture == Gestures::LongPress){
            // TODO
        }else if (gesture == Gestures::SlideLeft){
           // TODO
        }
        else if (gesture == Gestures::SlideRight){
            // TODO
        }
    }
    last_gesture = gesture;
}

void my_touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data )
{
    static bool last_touched = false;
    touch_drv.update();
    bool touched = touch_drv.tp.touching;
    if( !touched )
    {
        data->state = LV_INDEV_STATE_REL;
        last_touched = false;
    }
    else
    {
        data->state = LV_INDEV_STATE_PR;
        /*Change to your screen resolution*/
        data->point.x = touch_drv.tp.y;
        data->point.y = 240-touch_drv.tp.x;
        gesture_handler(touch_drv.tp.gesture);
    }
}

bool touch_scan(){
    Wire1.setSCL(TOUCH_SCL);
    Wire1.setSDA(TOUCH_SDA);
    for (uint8_t i = 0; i < 5; i++){
        Wire1.begin();
        Wire1.beginTransmission(I2C_ADDR_FT6336U);
        uint8_t error = Wire1.endTransmission();
        if (error != 0){
            Serial.printf("[%6lu]", millis());
            Serial.printf("[I2C] Tp not found, error: %d\n", error);
        }else{
            Serial.printf("[%6lu]", millis());
            Serial.printf("[I2C] Tp found: %d\n", error);
            return true;
            break;
        } 
        delay(100);
    }
    return false;
}

void touch_init(void){
    Serial.printf("[%6lu]", millis());
    Serial.println("[Touch] Touch setup");
    if (touch_scan() == false){
        Serial.printf("[%6lu]", millis());
        Serial.println("[Touch] Touch not found, please check the connection.");
        return;
    }
    touch_drv.begin();

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );
}

void touch_setup_nolvgl(void){
    touch_drv.begin();
}


#endif