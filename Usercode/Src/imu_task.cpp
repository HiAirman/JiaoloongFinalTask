//
// Created by HiAir on 2025/11/18.
//

#include "imu_task.h"

#include "cmsis_os2.h"


#include "gimbal_settings.h"
#include "data_types.h"
#include "user_tasks.h"
#include "imu.h"

osThreadId_t imu_task_handle;
osThreadAttr_t imu_task_attribute{
    .name = "imu_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_IMU_TASK,
};

[[noreturn]] void imu_task(void*) {
    //只能加入软件初始化
    float r_imu[3][3] = { { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 } };
    float g_bias[3] = IMU_GYRO_BIAS;
    IMU imu(IMU_CALCULATE_DELTA_TIME,
            IMU_FUSION_COEFFICIENT,
            IMU_G_THRESHOLD,
            r_imu,
            g_bias
    );
    uint32_t sequence = 0;
    //此处应该有读取imu数据得到初始位置角度
    imu.init();
    //给motor的init传输数据
    motor_initialize_data_t motor_initialize_data;
    motor_initialize_data.pitch_angle = imu.euler_deg_.pitch;
    motor_initialize_data.yaw_angle = 0;
    osMessageQueuePut(imu_to_motor_queue_handle, &motor_initialize_data, 0,osWaitForever);
    while (true) {
        auto ticks = osKernelGetTickCount();
        //spi读取数据
        imu.readSensor();
        //解算
        imu_data_t imu_data;
        imu.update();
        imu_data.pitch = imu.euler_deg_.pitch;
        imu_data.roll = imu.euler_deg_.roll;
        imu_data.yaw = imu.euler_deg_.yaw;
        imu_data.timestamp = osKernelGetTickCount();
        imu_data.sequence = ++sequence;
        //传输数据给control_task
        //get old ones (try semantics)
        imu_data_t unused_data;
        osMessageQueueGet(imu_to_control_queue_handle, &unused_data, nullptr, 0);
        //send (no delay
        osMessageQueuePut(imu_to_control_queue_handle, &imu_data, 0, 0);

        osDelayUntil(ticks + TASK_DELAY_TIME_IMU_TASK);
    }
}