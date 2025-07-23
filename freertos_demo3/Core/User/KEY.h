//
// Created by clx on 2025/7/21.
//

#ifndef FREERTOS_DEMO1_KEY_H
#define FREERTOS_DEMO1_KEY_H

#include <stdint.h>
#include "gpio.h"

#define KEY1_PRESS 1
#define KEY2_PRESS 2
#define KEY3_PRESS 3
#define KEY4_PRESS 4


uint8_t Key_Detect(void);

#endif //FREERTOS_DEMO1_KEY_H
