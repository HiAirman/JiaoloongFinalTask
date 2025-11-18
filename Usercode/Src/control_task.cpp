//
// Created by HiAir on 2025/11/18.
//

#include "control_task.h"

#include "cmsis_os2.h"

#include "gimbal_settings.h"
#include "data_types.h"
#include "user_tasks.h"

osThreadId_t control_task_handle;
osThreadAttr_t control_task_attribute{
    .name = "control_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_CONTROL_TASK,
};

[[noreturn]] void control_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();
        //接收遥控数据
        controller_data_t controller_data;
        osMessageQueueGet(dbus_to_control_queue_handle, &controller_data, nullptr, osWaitForever);
        //接收imu数据
        imu_data_t imu_data;
        osMessageQueueGet(imu_to_control_queue_handle, &imu_data, nullptr, osWaitForever);
        //计算目标位置
        motor_output_data_t motor_output_data, unused_data;
        //计算前馈角度
        //发给motor_task
        //get old ones (try semantics)
        osMessageQueueGet(control_to_motor_queue_handle, &unused_data, nullptr, 0);
        //send (no delay
        osMessageQueuePut(control_to_motor_queue_handle, &motor_output_data, 0, 0);

        osDelayUntil(ticks + TASK_DELAY_TIME_CONTROL_TASK);
    }
}