//
// Created by HiAir on 2025/11/18.
//

#ifndef JIAOLOONGFINALTASK_IMU_TASK_H
#define JIAOLOONGFINALTASK_IMU_TASK_H

#include "cmsis_os2.h"

extern osThreadId_t imu_task_handle;
extern osThreadAttr_t imu_task_attribute;

[[noreturn]] void imu_task(void*);

#endif //JIAOLOONGFINALTASK_IMU_TASK_H