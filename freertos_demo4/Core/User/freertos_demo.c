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

/* 任务1的配置 */
#define TASK1_STACK 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handle;

void task1(void *pvParameters);




/**
 * 启动freertos
 */
void freertos_start(void) {

    // 1.创建一个启动任务
    xTaskCreate(start_task, // 任务函数的地址
                "start_task", // 任务名称
                START_TASK_STACK, // 任务堆栈大小,默认最小128，单位4字节
                NULL, // 传递给任务的参数
                START_TASK_PRIORITY, // 任务优先级
                &start_task_handle); // 任务句柄的地址

    // 2.启动调度器：会自动创建空闲任务
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

    xTaskCreate(task1,
                "task1",
                TASK1_STACK,
                NULL,
                TASK1_PRIORITY,
                &task1_handle);

    // 离开临界区
    taskEXIT_CRITICAL();

    // 启动任务只需要执行一次即可，用完就删除自己
    vTaskDelete(NULL);
}

/**
 * 任务1：实现led1 500ms闪烁
 * @param pvParameters
 */
void task1(void *pvParameters) {
        printf("task1运行。。 。");


    uint8_t key = 0;
    while (1) {
        key = Key_Detect();
        if(key == KEY1_PRESS) {
            // 关中断
            printf("关中断\n");
            portDISABLE_INTERRUPTS();
        } else if (key == KEY2_PRESS) {
            printf("开中断\n");
            // 开中断
            portENABLE_INTERRUPTS();
        } 
    }
}

