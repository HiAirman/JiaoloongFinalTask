//
// Created by HiAir on 2025/11/18.
//

#ifndef JIAOLOONGFINALTASK_CAN_TX_TASK_H
#define JIAOLOONGFINALTASK_CAN_TX_TASK_H

#include "cmsis_os2.h"

#include "gimbal_settings.h"

osThreadId_t can_tx_task_handle;
osThreadAttr_t can_tx_task_attribute{
    .name = "can_tx_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_CAN_TX_TASK,
};

[[noreturn]] void can_tx_task(void*);
#endif //JIAOLOONGFINALTASK_CAN_TX_TASK_H