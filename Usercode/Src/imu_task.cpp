//
// Created by HiAir on 2025/11/18.
//

#include "imu_task.h"

#include "cmsis_os2.h"


#include "gimbal_settings.h"
#include "data_types.h"
#include "user_tasks.h"

osThreadId_t imu_task_handle;
osThreadAttr_t imu_task_attribute{
    .name = "imu_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_IMU_TASK,
};

[[noreturn]] void imu_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();
        //spi读取数据
        //解算
        imu_data_t imu_data, unused_data;
        //传输数据给control_task
        //get old ones (try semantics)
        osMessageQueueGet(control_to_motor_queue_handle, &unused_data, nullptr, 0);
        //send (no delay
        osMessageQueuePut(control_to_motor_queue_handle, &imu_data, 0, 0);

        osDelayUntil(ticks + TASK_DELAY_TIME_IMU_TASK);
    }
}