//
// Created by HiAir on 2025/11/18.
//

#include "control_task.h"

#include "cmsis_os2.h"

#include "gimbal_settings.h"


[[noreturn]] void control_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();

        osDelayUntil(ticks + TASK_DELAY_TIME_CONTROL_TASK);
    }
}