//
// Created by HiAir on 2025/11/24.
//

#include "controller.h"

#include "cmsis_os2.h"
#include "usart.h"
#include "user_tasks.h"

void Controller::init() {
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx_data_, 32);
}


void Controller::dbus_isr_callback(uint16_t size) {
    //接收uart数据
    if (size != 16) {
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx_data_, 32);
        return;
    }
    //解析数据 chx: -1 ~ +1 swx: -1 0 1
    controller_data_.ch0 = ((((int16_t)rx_data_[0] | ((int16_t)rx_data_[1] << 8)) & 0x07FF) - RC_CH_VALUE_OFFSET) * (2.f
        / (RC_CH_VALUE_MAX - RC_CH_VALUE_MIN));
    controller_data_.ch1 = (((((int16_t)rx_data_[1] >> 3) | ((int16_t)rx_data_[2] << 5)) & 0x07FF) - RC_CH_VALUE_OFFSET)
        * (2.f / (RC_CH_VALUE_MAX - RC_CH_VALUE_MIN));
    controller_data_.ch2 = (((((int16_t)rx_data_[2] >> 6) | ((int16_t)rx_data_[3] << 2) | ((int16_t)rx_data_[4] << 10))
        & 0x07FF) - RC_CH_VALUE_OFFSET) * (2 / (RC_CH_VALUE_MAX - RC_CH_VALUE_MIN));
    controller_data_.ch3 = (((((int16_t)rx_data_[4] >> 1) | ((int16_t)rx_data_[5] << 7)) & 0x07FF) - RC_CH_VALUE_OFFSET)
        * (2 / (RC_CH_VALUE_MAX - RC_CH_VALUE_MIN));
    uint8_t sw1_temp = ((rx_data_[5] >> 4) & 0x000C) >> 2;
    uint8_t sw2_temp = ((rx_data_[5] >> 5) & 0x0003);
    if (sw1_temp == RC_SW_UP) {
        controller_data_.sw1 = CONTROLLER_SW_UP;
        sw1_flag_ = 1;
    } else if (sw1_temp == RC_SW_MID) {
        controller_data_.sw1 = CONTROLLER_SW_MID;
        sw1_flag_ = 0;
    } else if (sw1_temp == RC_SW_DOWN) {
        controller_data_.sw1 = CONTROLLER_SW_DOWN;
        sw1_flag_ = -1;
    }
    if (sw2_temp == RC_SW_UP) {
        controller_data_.sw2 = CONTROLLER_SW_UP;
    } else if (sw2_temp == RC_SW_MID) {
        controller_data_.sw2 = CONTROLLER_SW_MID;
    } else if (sw2_temp == RC_SW_DOWN) {
        controller_data_.sw2 = CONTROLLER_SW_DOWN;
    }
    controller_data_.timestamp = HAL_GetTick();
    controller_data_.sequence++;
    //发送数据包
    // to control task
    controller_data_t unused_data;
    osMessageQueueGet(dbus_to_control_queue_handle, &unused_data, nullptr, 0);
    osMessageQueuePut(dbus_to_control_queue_handle, &controller_data_, 0, 0);
    // to motor task
    uint8_t unused_flag;
    osMessageQueueGet(dbus_to_motor_queue_handle, &unused_flag, nullptr, 0);
    osMessageQueuePut(dbus_to_motor_queue_handle, &sw1_flag_, 0, 0);
    //重新调用接收
    HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rx_data_, 32);
}

bool Controller::is_controller_connected() {
    return HAL_GetTick() - 50 < controller_data_.timestamp;
}


Controller controller;