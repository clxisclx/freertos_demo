//
// Created by clx on 2025/7/21.
//
#include "KEY.h"


uint8_t Key_Detect(void){
    uint8_t res = 0;
    if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_RESET){
        HAL_Delay(10);// 去抖动
        if(HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin) == GPIO_PIN_RESET) {
            res = 1;
        }
    }
    return res;
}
