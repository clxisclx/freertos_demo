//
// Created by clx on 2025/7/21.
//

#include "stdio.h"
#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "LED.h"
#include "KEY.h"

#include <stdlib.h>

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

/* 任务2的配置 */
#define TASK2_STACK 128
#define TASK2_PRIORITY 3
TaskHandle_t task2_handle;

void task2(void *pvParameters);

/* 任务3的配置 */
#define TASK3_STACK 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handle;

void task3(void *pvParameters);


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

    xTaskCreate(task2,
                "task2",
                TASK2_STACK,
                NULL,
                TASK2_PRIORITY,
                &task2_handle);

    xTaskCreate(task3,
                "task3",
                TASK3_STACK,
                NULL,
                TASK3_PRIORITY,
                &task3_handle);

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
    while (1) {
         printf("task1运行。。。\n");
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
        printf("task2运行。。。\n");
        LED_Toggle(LED2_GPIO_Port, LED2_Pin);
        vTaskDelay(500);
    }
}


/**
 * 
 * @param pvParameters
 */

// char task_info[500]; // 可以
void task3(void *pvParameters) {

    // char task_info[500]; // 栈溢出 
    // char* task_info = (char *)malloc(100*sizeof(char)); // 可以，但使用的是系统堆，不依赖FreeRTOS的堆管理器
    char* task_info = (char*)pvPortMalloc(40 * uxTaskGetNumberOfTasks()); // 推荐，大小为 40*任务数量
    uint8_t key = 0;
    uint16_t is_suspend_all = 0;
    while (1) {
        printf("task3运行。。。\n");
        key = Key_Detect();


        if (key == KEY1_PRESS) { // key1按下，挂起task1
            printf("task1挂起。。。\n");
            vTaskSuspend(task1_handle);
        } else if (key == KEY2_PRESS) { // key2按下,恢复task1
            printf("task1恢复。。。\n");
            vTaskResume(task1_handle);
        } else if (key == KEY3_PRESS) { // key3按下，挂起调度器
            printf("调度器挂起。。。\n");
            vTaskSuspendAll(); // 挂起调度器
            is_suspend_all = 1;
        } else if (key == KEY4_PRESS) { // key4按下，恢复调度器
            printf("调度器恢复。。。\n");
            xTaskResumeAll(); // 恢复调度器
            is_suspend_all = 0;
        }

        // 打印任务状态
        // 
       vTaskList(task_info);
       // 输出格式为：名称  状态  优先级  堆栈使用  任务编号
       // 运行状态：X:运行，B:挂起，R:就绪，S:暂定，D:删除
       printf("任务状态：\n%s\n", task_info);

       // 当调度器挂起时，延时函数无效
       if(!is_suspend_all){ 
            vTaskDelay(500);
       }
    }
    // free(task_info);
    vPortFree(task_info);
}

