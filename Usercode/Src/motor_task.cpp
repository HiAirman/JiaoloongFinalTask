//
// Created by HiAir on 2025/11/18.
//

#include "motor_task.h"

#include "cmsis_os2.h"

#include "gimbal_settings.h"

osThreadId_t motor_task_handle;
osThreadAttr_t motor_task_attribute{
    .name = "motor_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_MOTOR_TASK,
};

[[noreturn]] void motor_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();

        osDelayUntil(ticks + TASK_DELAY_TIME_MOTOR_TASK);
    }
}