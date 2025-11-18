//
// Created by HiAir on 2025/11/18.
//

#include "motor_task.h"

#include "cmsis_os2.h"

#include "gimbal_settings.h"

[[noreturn]] void motor_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();

        osDelayUntil(ticks + TASK_DELAY_TIME_MOTOR_TASK);
    }
}