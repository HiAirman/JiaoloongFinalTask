//
// Created by HiAir on 2025/11/18.
//

#include "can_tx_task.h"

#include "cmsis_os2.h"


#include "gimbal_settings.h"
#include "data_types.h"
#include "user_tasks.h"

osThreadId_t can_tx_task_handle;
osThreadAttr_t can_tx_task_attribute{
    .name = "can_tx_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_CAN_TX_TASK,
};

[[noreturn]] void can_tx_task(void*) {
    while (true) {
        // auto ticks = osKernelGetTickCount();
        //阻塞式等待motor的数据
        motor_output_data_t output_data;
        osMessageQueueGet(motor_to_can_tx_queue_handle, &output_data, nullptr, osWaitForever);
        //发送can_tx包

        //不需要delay
        // osDelayUntil(ticks + TASK_DELAY_TIME_CAN_TX_TASK);
    }
}