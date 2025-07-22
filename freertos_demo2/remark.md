静态创建任务和删除


1.开启静态任务配置：
注：使用cubemx配置时，这两个自动配置过了
```c
// 。。。。


// 开启静态分配
#define configSUPPORT_STATIC_ALLOCATION          1
// 开启软件定时器
#define configUSE_TIMERS                         1
```

手动实现两个方法：vApplicationGetIdleTaskMemory 和 vApplicationGetTimerTaskMemory
另外任务句柄从形参变成了赋值
```c
    // 任务句柄赋值
    task1_handle = xTaskCreateStatic(task1,
                                     "task1",
                                     TASK1_STACK,
                                     NULL,
                                     TASK1_PRIORITY,
                                     task1_stack,
                                     &task1_TCB);
```

