//
// Created by clx on 2025/7/21.
//

#ifndef FREERTOS_DEMO1_LED_H
#define FREERTOS_DEMO1_LED_H

#include "gpio.h"

void LED_Turn_On(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void LED_Turn_Off(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void LED_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);

void LED_Turn_Off_All(void);


#endif //FREERTOS_DEMO1_LED_H
