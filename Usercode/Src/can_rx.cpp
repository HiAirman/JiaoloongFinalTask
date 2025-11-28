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
        .FilterIdHigh = (0x204 + MOTOR_PITCH_ID) << 5, // first one for 16 bit mode
        .FilterIdLow = (0x204 + MOTOR_YAW_ID) << 5, // second one for 16 bits mode
        .FilterMaskIdHigh = 0x7FF << 5, // mask: we only care about first 11 bits
        .FilterMaskIdLow = 0x7FF << 5, // mask: we only care about first 11 bits
        .FilterFIFOAssignment = CAN_FILTER_FIFO0,
        .FilterBank = 0, // filter bank NO.0
        .FilterMode = CAN_FILTERMODE_IDMASK,
        .FilterScale = CAN_FILTERSCALE_16BIT,
        .FilterActivation = ENABLE
    };
    HAL_CAN_ConfigFilter(&hcan1, &filter);
    HAL_CAN_Start(&hcan1);
    can_rx_interrupt_init();
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
    if (rx_header.StdId == 0x204 + MOTOR_PITCH_ID) {
        feedback_data.updated = -1;
        feedback_data.ecd_angle_pitch = (int16_t)(rx_data[0] << 8 | rx_data[1]) / 8191.0f * 360.0f;
        feedback_data.ecd_angular_velocity_pitch = (int16_t)(rx_data[2] << 8 | rx_data[3]) / 60.0f * 360.0f;
        feedback_data.real_current_pitch = (int16_t)(rx_data[4] << 8 | rx_data[5]) / 16384.0f * 3.0f;
        feedback_data.temperature_pitch = (int16_t)(rx_data[6]);
    }
    if (rx_header.StdId == 0x204 + MOTOR_YAW_ID) {
        feedback_data.updated = 1;
        feedback_data.ecd_angle_yaw = (int16_t)(rx_data[0] << 8 | rx_data[1]) / 8191.0f * 360.0f;
        feedback_data.ecd_angular_velocity_yaw = (int16_t)(rx_data[2] << 8 | rx_data[3]) / 60.0f * 360.0f;
        feedback_data.real_current_yaw = (int16_t)(rx_data[4] << 8 | rx_data[5]) / 16384.0f * 3.0f;
        feedback_data.temperature_yaw = (int16_t)(rx_data[6]);
    }
    feedback_data.timestamp = osKernelGetTickCount();
    feedback_data.sequence++;
    //发包给motor_task
    motor_feedback_data_t unused_data;
    osMessageQueueGet(can_rx_to_motor_queue_handle, &unused_data, nullptr, 0);
    osMessageQueuePut(can_rx_to_motor_queue_handle, &feedback_data, 0, 0);
}