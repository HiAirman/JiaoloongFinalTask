//
// Created by HiAir on 2025/11/18.
//

#include "motor_task.h"

#include "cmsis_os2.h"

#include "gimbal_settings.h"
#include "data_types.h"
#include "motor.h"
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

float calculate_pitch_feedforward(float pitch_angle) {
    return 0.0;
}

[[noreturn]] void motor_task(void*) {
    //initialize part
    Motor pitch_motor, yaw_motor;
    pitch_motor = Motor(MOTOR_PITCH_POSITION_PID_KP,
                        MOTOR_PITCH_POSITION_PID_KI,
                        MOTOR_PITCH_POSITION_PID_KD,
                        MOTOR_PITCH_POSITION_PID_I_MAX,
                        MOTOR_PITCH_POSITION_PID_OUT_MAX,
                        MOTOR_PITCH_SPEED_PID_KP,
                        MOTOR_PITCH_SPEED_PID_KI,
                        MOTOR_PITCH_SPEED_PID_KD,
                        MOTOR_PITCH_SPEED_PID_I_MAX,
                        MOTOR_PITCH_SPEED_PID_OUT_MAX);
    yaw_motor = Motor(MOTOR_YAW_POSITION_PID_KP,
                      MOTOR_YAW_POSITION_PID_KI,
                      MOTOR_YAW_POSITION_PID_KD,
                      MOTOR_YAW_POSITION_PID_I_MAX,
                      MOTOR_YAW_POSITION_PID_OUT_MAX,
                      MOTOR_YAW_SPEED_PID_KP,
                      MOTOR_YAW_SPEED_PID_KI,
                      MOTOR_YAW_SPEED_PID_KD,
                      MOTOR_YAW_SPEED_PID_I_MAX,
                      MOTOR_YAW_SPEED_PID_OUT_MAX);

    motor_initialize_data_t motor_initialize_data;
    osMessageQueueGet(imu_to_motor_queue_handle, &motor_initialize_data, nullptr,osWaitForever);
    pitch_motor.init(motor_initialize_data.pitch_angle);
    yaw_motor.init(motor_initialize_data.yaw_angle);

    while (true) {
        auto ticks = osKernelGetTickCount();
        //从can_rx_isr接收反馈角度数据
        motor_feedback_data_t feedback_data;
        osMessageQueueGet(can_rx_to_motor_queue_handle, &feedback_data, nullptr, osWaitForever);
        if (feedback_data.updated == -1) {
            pitch_angle = feedback_data.ecd_angle_pitch;
            pitch_angular_velocity = feedback_data.ecd_angular_velocity_pitch;
        }
        if (feedback_data.updated == 1) {
            yaw_angle = feedback_data.ecd_angle_yaw;
            yaw_angular_velocity = feedback_data.ecd_angular_velocity_yaw;
        }
        //从dbus_isr接收右拨杆使能
        int8_t flag;
        osMessageQueueGet(dbus_to_motor_queue_handle, &flag, nullptr, 0);
        //从control_task接收目标位置和前馈数据
        motor_control_data_t motor_control_data;
        //test
        motor_control_data.pitch_motor_position = 0.0;
        motor_control_data.yaw_motor_position = 0.0;

        //Motor main loop
        motor_output_data_t output_data;
        //pitch
        pitch_motor.set_target(motor_control_data.pitch_motor_position);
        pitch_motor.set_feedback(pitch_angle, pitch_angular_velocity);
        pitch_motor.set_feedforward_torque(calculate_pitch_feedforward(pitch_angle));
        output_data.pitch_motor_current = pitch_motor.get_current();
        //yaw
        yaw_motor.set_target(motor_control_data.yaw_motor_position);
        yaw_motor.set_feedback(yaw_angle, yaw_angular_velocity);
        yaw_motor.set_feedforward_torque(0.0f);
        output_data.yaw_motor_current = yaw_motor.get_current();

        //给can_tx_task传输输出电流值
        if (flag == -1 || flag == 0) {
            // DOWN or MID 取消使能
            output_data.pitch_motor_current = 0.0;
            output_data.yaw_motor_current = 0.0;
        }
        output_data.timestamp = osKernelGetTickCount();
        output_data.sequence++;
        osMessageQueuePut(motor_to_can_tx_queue_handle, &output_data, 0, 0);

        //安全delay
        if (ticks + TASK_DELAY_TIME_MOTOR_TASK > osKernelGetTickCount()) {
            osDelayUntil(ticks + TASK_DELAY_TIME_MOTOR_TASK);
        }
    }
}