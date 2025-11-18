//
// Created by HiAir on 2025/11/18.
//

#ifndef JIAOLOONGFINALTASK_MOTOR_TASK_H
#define JIAOLOONGFINALTASK_MOTOR_TASK_H

#include "cmsis_os2.h"

extern osThreadId_t motor_task_handle;
extern osThreadAttr_t motor_task_attribute;

[[noreturn]] void motor_task(void*);

#endif //JIAOLOONGFINALTASK_MOTOR_TASK_H