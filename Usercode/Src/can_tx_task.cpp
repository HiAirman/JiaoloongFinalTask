//
// Created by HiAir on 2025/11/18.
//

#include "can_tx_task.h"

#include "cmsis_os2.h"

#include "gimbal_settings.h"

osThreadId_t can_tx_task_handle;
osThreadAttr_t can_tx_task_attribute{
    .name = "can_tx_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_CAN_TX_TASK,
};

[[noreturn]] void can_tx_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();

        osDelayUntil(ticks + TASK_DELAY_TIME_CAN_TX_TASK);
    }
}