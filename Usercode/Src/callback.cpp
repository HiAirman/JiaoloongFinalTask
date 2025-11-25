//
// Created by HiAir on 2025/11/24.
//
#include "main.h"
#include "controller.h"

//dbus_isr
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef* huart, uint16_t Size) {
    if (huart->Instance == USART3) {
        //dbus
        controller.dbus_isr_callback(Size);
    }
}