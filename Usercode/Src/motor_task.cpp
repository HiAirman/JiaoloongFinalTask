//
// Created by HiAir on 2025/11/18.
//

#include "motor_task.h"

#include "cmsis_os2.h"

#include "gimbal_settings.h"
#include "data_types.h"
#include "user_tasks.h"

osThreadId_t motor_task_handle;
osThreadAttr_t motor_task_attribute{
    .name = "motor_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_MOTOR_TASK,
};

//test
float pitch_angle, yaw_angle;
float pitch_angular_velocity, yaw_angular_velocity;
float pitch_current, yaw_current;

[[noreturn]] void motor_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();
        //从can_rx_isr接收反馈角度数据
        motor_feedback_data_t feedback_data;
        osMessageQueueGet(can_rx_to_motor_queue_handle, &feedback_data, nullptr, osWaitForever);
        if (feedback_data.updated = -1) {
            pitch_angle = feedback_data.ecd_angle_pitch;
            pitch_angular_velocity = feedback_data.ecd_angular_velocity_pitch;
        }
        if (feedback_data.updated = 1) {
            yaw_angle = feedback_data.ecd_angle_yaw;
            yaw_angular_velocity = feedback_data.ecd_angular_velocity_yaw;
        }
        //从dbus_isr接收右拨杆使能
        int8_t flag;
        osMessageQueueGet(dbus_to_motor_queue_handle, &flag, nullptr, 0);
        //从control_task接收目标位置和前馈数据

        motor_output_data_t output_data;
        //给can_tx_task传输输出电流值
        output_data.yaw_motor_current = 0.0;
        if (flag == 1) {
            // UP
            output_data.pitch_motor_current = 0.2;
        } else if (flag == -1) {
            // DOWN
            output_data.pitch_motor_current = 0.0;
        }
        output_data.timestamp = osKernelGetTickCount();
        output_data.sequence++;

        osMessageQueuePut(motor_to_can_tx_queue_handle, &output_data, 0, 0);
        if (ticks + TASK_DELAY_TIME_MOTOR_TASK > osKernelGetTickCount()) {
            osDelayUntil(ticks + TASK_DELAY_TIME_MOTOR_TASK);
        }
    }
}