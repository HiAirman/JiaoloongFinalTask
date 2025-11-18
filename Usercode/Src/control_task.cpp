//
// Created by HiAir on 2025/11/18.
//

#include "control_task.h"

#include "cmsis_os2.h"

#include "gimbal_settings.h"

osThreadId_t control_task_handle;
osThreadAttr_t control_task_attribute{
    .name = "control_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_CONTROL_TASK,
};

[[noreturn]] void control_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();

        osDelayUntil(ticks + TASK_DELAY_TIME_CONTROL_TASK);
    }
}