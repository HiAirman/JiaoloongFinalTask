//
// Created by HiAir on 2025/11/18.
//

#include "control_task.h"

#include "cmsis_os2.h"
#include "controller.h"

#include "gimbal_settings.h"
#include "data_types.h"
#include "user_tasks.h"

float pitch, roll, yaw;
float chx[4];
ControllerSwState swx[2];

osThreadId_t control_task_handle;
osThreadAttr_t control_task_attribute{
    .name = "control_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_CONTROL_TASK,
};

[[noreturn]] void control_task(void*) {
    //testing
    while (true) {
        auto ticks = osKernelGetTickCount();
        imu_data_t imu_data;
        osMessageQueueGet(imu_to_control_queue_handle, &imu_data, nullptr, osWaitForever);
        pitch = imu_data.pitch;
        roll = imu_data.roll;
        yaw = imu_data.yaw;
        //if (controller.is_controller_connected()) {
        controller_data_t controller_data;

        osMessageQueueGet(dbus_to_control_queue_handle, &controller_data, nullptr, osWaitForever);
        chx[0] = controller_data.ch0;
        chx[1] = controller_data.ch1;
        chx[2] = controller_data.ch2;
        chx[3] = controller_data.ch3;
        swx[0] = controller_data.sw1;
        swx[1] = controller_data.sw2;
        //}
        //osDelayUntil(ticks + TASK_DELAY_TIME_CONTROL_TASK);
    }
    while (true) {
        auto ticks = osKernelGetTickCount();
        //接收遥控数据
        controller_data_t controller_data;
        osMessageQueueGet(dbus_to_control_queue_handle, &controller_data, nullptr, osWaitForever);
        //接收imu数据
        imu_data_t imu_data;
        osMessageQueueGet(imu_to_control_queue_handle, &imu_data, nullptr, osWaitForever);
        //计算目标位置
        motor_control_data_t motor_control_data, unused_data;
        motor_control_data.pitch_motor_position = controller_data.ch1 * CONTROL_PITCH_RANGE;
        if (motor_control_data.pitch_motor_position >= CONTROL_PITCH_MAX) {
            motor_control_data.pitch_motor_position = CONTROL_PITCH_MAX;
        }
        if (motor_control_data.pitch_motor_position <= CONTROL_PITCH_MIN) {
            motor_control_data.pitch_motor_position = CONTROL_PITCH_MIN;
        }
        motor_control_data.yaw_motor_position = controller_data.ch0 * CONTROL_YAW_RANGE;
        //发给motor_task
        //get old ones (try semantics)
        osMessageQueueGet(control_to_motor_queue_handle, &unused_data, nullptr, 0);
        //send (no delay
        osMessageQueuePut(control_to_motor_queue_handle, &motor_control_data, 0, 0);

        osDelayUntil(ticks + TASK_DELAY_TIME_CONTROL_TASK);
    }
}