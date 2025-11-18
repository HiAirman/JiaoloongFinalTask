//
// Created by HiAir on 2025/11/18.
//

#include "imu_task.h"

#include "cmsis_os2.h"

#include "gimbal_settings.h"

osThreadId_t imu_task_handle;
osThreadAttr_t imu_task_attribute{
    .name = "imu_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_IMU_TASK,
};

[[noreturn]] void imu_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();

        osDelayUntil(ticks + TASK_DELAY_TIME_IMU_TASK);
    }
}