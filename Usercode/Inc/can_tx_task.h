//
// Created by HiAir on 2025/11/18.
//

#ifndef JIAOLOONGFINALTASK_CAN_TX_TASK_H
#define JIAOLOONGFINALTASK_CAN_TX_TASK_H

#include "cmsis_os2.h"

extern osThreadId_t can_tx_task_handle;
extern osThreadAttr_t can_tx_task_attribute;

[[noreturn]] void can_tx_task(void*);
#endif //JIAOLOONGFINALTASK_CAN_TX_TASK_H