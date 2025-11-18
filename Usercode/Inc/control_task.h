//
// Created by HiAir on 2025/11/18.
//

#ifndef JIAOLOONGFINALTASK_CONTROL_TASK_H
#define JIAOLOONGFINALTASK_CONTROL_TASK_H

#include "cmsis_os2.h"

extern osThreadId_t control_task_handle;
extern osThreadAttr_t control_task_attribute;

//主控制循环
[[noreturn]] void control_task(void*);

#endif //JIAOLOONGFINALTASK_CONTROL_TASK_H