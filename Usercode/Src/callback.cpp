//
// Created by HiAir on 2025/11/24.
//
#include "main.h"
#include "controller.h"
#include "can_rx.h"

//dbus_isr
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    if (huart->Instance == USART3) {
        //dbus
        controller.dbus_isr(Size);
    }
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan) {
    if (hcan->Instance == CAN1) {
        can_rx_isr();
    }
}