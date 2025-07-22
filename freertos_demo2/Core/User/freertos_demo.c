//
// Created by clx on 2025/7/21.
//

#include "stdio.h"
#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "KEY.h"

/* 启动任务的配置 */
#define START_TASK_STACK 128
#define START_TASK_PRIORITY 1
TaskHandle_t start_task_handle;

void start_task(void *pvParameters);

StackType_t start_task_stack[START_TASK_STACK];
StaticTask_t start_task_TCB;

/* 任务1的配置 */
#define TASK1_STACK 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handle;

void task1(void *pvParameters);

StackType_t task1_stack[TASK1_STACK];
StaticTask_t task1_TCB;


/* 任务2的配置 */
#define TASK2_STACK 128
#define TASK2_PRIORITY 3
TaskHandle_t task2_handle;

void task2(void *pvParameters);

StackType_t task2_stack[TASK2_STACK];
StaticTask_t task2_TCB;

/* 任务3的配置 */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handle;

void task3(void *pvParameters);

StackType_t task3_stack[TASK3_STACK];
StaticTask_t task3_TCB;


/* ============================= 静态创建方式，需要手动指定2个特殊任务的资源 ============================= */
/* 空闲任务的配置 */
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE]; // 静态任务的任务栈，以数组形式存储
StaticTask_t idle_task_tcb; // 静态任务的TCB结构体类型

/* 软件定时器任务的配置 */
StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH]; // 定时任务的任务栈，以数组形式存储
StaticTask_t timer_task_tcb; // 定时任务的TCB结构体类型



// 分配空闲任务的资源
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
    *ppxIdleTaskTCBBuffer = &idle_task_tcb;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

// 分配软件定时器任务的资源
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize) {
    *ppxTimerTaskTCBBuffer = &timer_task_tcb;
    *ppxTimerTaskStackBuffer = timer_task_stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/* ==================================================================================================== */


/**
 * 启动freertos
 */
void freertos_start(void) {

    // 1.创建一个启动任务
    start_task_handle = xTaskCreateStatic(start_task,
                                          "start_task",
                                          START_TASK_STACK,
                                          NULL,
                                          START_TASK_PRIORITY,
                                          start_task_stack,// 任务栈的地址
                                          &start_task_TCB// 任务控制块的地址
    );
    // 2.启动调度器：会自动创建空闲任务和软件定时器(如果开启的话)，静态创建的方式需要去实现2个分片资源的接口函数
    vTaskStartScheduler();
}

/**
 * 启动任务:用来创建其他task
 * @param pvParameters
 */
void start_task(void *pvParameters) {

    // 进入临界区: 临界区中的代码不会被打断
    // 此处如果没有临界区，当创建完task1后，因为task1的优先级2高于start_task优先级1，所以会执行task1任务，直到运行task1中的delay,才继续执行start_task
    taskENTER_CRITICAL();

    task1_handle = xTaskCreateStatic(task1,
                                     "task1",
                                     TASK1_STACK,
                                     NULL,
                                     TASK1_PRIORITY,
                                     task1_stack,
                                     &task1_TCB);

    task2_handle = xTaskCreateStatic(task2,
                                     "task2",
                                     TASK2_STACK,
                                     NULL,
                                     TASK2_PRIORITY,
                                     task2_stack,
                                     &task2_TCB);

    task2_handle = xTaskCreateStatic(task3,
                                     "task3",
                                     TASK3_STACK,
                                     NULL,
                                     TASK3_PRIORITY,
                                     task3_stack,
                                     &task3_TCB);

    // 启动任务只需要执行一次即可，用完就删除自己
    vTaskDelete(NULL);

    // 离开临界区
    taskEXIT_CRITICAL();
}

/**
 * 任务1：实现led1 500ms闪烁
 * @param pvParameters
 */
void task1(void *pvParameters) {
    while (1) {
        printf("task1运行。。。");
        LED_Toggle(LED1_GPIO_Port, LED1_Pin);
        vTaskDelay(500);
    }
}

/**
 * 任务2：实现led2 500ms闪烁
 * @param pvParameters
 */
void task2(void *pvParameters) {
    while (1) {
        printf("task2运行。。。");
        LED_Toggle(LED2_GPIO_Port, LED2_Pin);
        vTaskDelay(500);
    }
}


/**
 * 任务3：判断key1是否按下，按下则删除task1
 * @param pvParameters
 */
void task3(void *pvParameters) {
    printf("task3运行。。。");
    uint8_t key = 0;
    while (1) {
        key = Key_Detect();
        if (key == KEY1_PRESS && task1_handle) {
            printf("key1按下，删除task1");
            vTaskDelete(task1_handle);
            task1_handle = NULL;
        }
        // task3优先级最高，如果不加延时，task1和2永远无法执行
        // 使用HAL_Delay不会让出cpu
        vTaskDelay(500);
    }
}

