//
// Created by HiAir on 2025/11/18.
//

#ifndef JIAOLOONGFINALTASK_IMU_TASK_H
#define JIAOLOONGFINALTASK_IMU_TASK_H

#include "cmsis_os2.h"

#include "gimbal_settings.h"

osThreadId_t imu_task_handle;
osThreadAttr_t imu_task_attribute{
    .name = "imu_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_IMU_TASK,
};

[[noreturn]] void imu_task(void*);

#endif //JIAOLOONGFINALTASK_IMU_TASK_H