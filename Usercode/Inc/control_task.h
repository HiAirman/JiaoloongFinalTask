//
// Created by HiAir on 2025/11/18.
//

#ifndef JIAOLOONGFINALTASK_CONTROL_TASK_H
#define JIAOLOONGFINALTASK_CONTROL_TASK_H

#include "cmsis_os2.h"

#include "gimbal_settings.h"

osThreadId_t control_task_handle;
osThreadAttr_t control_task_attribute{
    .name = "control_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_CONTROL_TASK,
};

//主控制循环
[[noreturn]] void control_task(void*);

#endif //JIAOLOONGFINALTASK_CONTROL_TASK_H