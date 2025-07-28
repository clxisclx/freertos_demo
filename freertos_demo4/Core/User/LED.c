//
// Created by clx on 2025/7/21.
//

#include "LED.h"

void LED_Turn_On(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_SET);
}

void LED_Turn_Off(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
    HAL_GPIO_WritePin(GPIOx, GPIO_Pin, GPIO_PIN_RESET);
}

void LED_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin){
    HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}

void LED_Turn_Off_All(void){
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1, GPIO_PIN_RESET);
}
