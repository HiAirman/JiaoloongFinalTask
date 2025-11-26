//
// Created by HiAir on 2025/11/26.
//

#include "can.h"
#include "can_rx.h"

#include "cmsis_os2.h"
#include "data_types.h"
#include "gimbal_settings.h"
#include "user_tasks.h"

motor_feedback_data_t feedback_data;

void can_rx_init() {
    CAN_FilterTypeDef filter = {
        .FilterIdHigh = 0x0204 + MOTOR_PITCH_ID, // first one for 16 bit mode
        .FilterIdLow = 0x0204 + MOTOR_YAW_ID, // second one for 16 bits mode
    };
    HAL_CAN_ConfigFilter(&hcan1, &filter);
    HAL_CAN_Start(&hcan1);
}

void can_rx_interrupt_init() {
    HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
}


void can_rx_isr() {
    //读包
    CAN_RxHeaderTypeDef rx_header;
    uint8_t rx_data[8];
    HAL_CAN_GetRxMessage(&hcan1,CAN_RX_FIFO0, &rx_header, rx_data);
    //解包 Update feedback data
    if (rx_header.StdId = 0x204 + MOTOR_PITCH_ID) {
        feedback_data.ecd_angle_pitch = (rx_data[0] << 8 | rx_data[1]) / 8191 * 360;
        feedback_data.ecd_angular_velocity_pitch = (rx_data[2] << 8 | rx_data[3]) / 360 * 60;
        feedback_data.real_current_pitch = (rx_data[4] << 8 | rx_data[5]) / 16384 * 3;
        feedback_data.temperature_pitch = (rx_data[6]);
    }
    if (rx_header.StdId = 0x204 + MOTOR_YAW_ID) {
        feedback_data.ecd_angle_yaw = (rx_data[0] << 8 | rx_data[1]) / 8191 * 360;
        feedback_data.ecd_angular_velocity_yaw = (rx_data[2] << 8 | rx_data[3]) / 360 * 60;
        feedback_data.real_current_yaw = (rx_data[4] << 8 | rx_data[5]) / 16384 * 3;
        feedback_data.temperature_yaw = (rx_data[6]);
    }
    //发包给motor_task
    motor_feedback_data_t unused_data;
    osMessageQueueGet(can_rx_to_motor_queue_handle, &unused_data, nullptr, 0);
    osMessageQueuePut(can_rx_to_motor_queue_handle, &feedback_data, 0, 0);
}