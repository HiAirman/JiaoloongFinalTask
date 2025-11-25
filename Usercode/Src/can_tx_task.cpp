//
// Created by HiAir on 2025/11/18.
//

#include "can_tx_task.h"

#include "cmsis_os2.h"

#include "can.h"
#include "gimbal_settings.h"
#include "data_types.h"
#include "user_tasks.h"

osThreadId_t can_tx_task_handle;
osThreadAttr_t can_tx_task_attribute{
    .name = "can_tx_task",
    .stack_size = TASK_STACK_SIZE_DEFAULT,
    .priority = TASK_PRIORITY_CAN_TX_TASK,
};

[[noreturn]] void can_tx_task(void*) {
    while (true) {
        // auto ticks = osKernelGetTickCount();
        //阻塞式等待motor的数据
        motor_output_data_t output_data;
        osMessageQueueGet(motor_to_can_tx_queue_handle, &output_data, nullptr, osWaitForever);
        //发送can_tx包
        uint16_t pitch_motor_data = (uint16_t)(int16_t)(output_data.pitch_motor_current / 3 * 16384);
        uint16_t yaw_motor_data = (uint16_t)(int16_t)(output_data.yaw_motor_current / 3 * 16384);
        uint8_t tx_data1[8] = { 0 }, tx_data2[8] = { 0 };
        if (MOTOR_PITCH_ID <= 4) {
            tx_data1[(MOTOR_PITCH_ID - 1) * 2] = (uint8_t)(pitch_motor_data >> 8); // MSB
            tx_data1[(MOTOR_PITCH_ID - 1) * 2 + 1] = (uint8_t)(pitch_motor_data & 0xFF); // LSB
        } else {
            tx_data2[(MOTOR_PITCH_ID - 4 - 1) * 2] = (uint8_t)(pitch_motor_data >> 8); // MSB
            tx_data2[(MOTOR_PITCH_ID - 4 - 1) * 2 + 1] = (uint8_t)(pitch_motor_data & 0xFF); // LSB
        }
        if (MOTOR_YAW_ID <= 4) {
            tx_data1[(MOTOR_YAW_ID - 1) * 2] = (uint8_t)(yaw_motor_data >> 8); // MSB
            tx_data1[(MOTOR_YAW_ID - 1) * 2 + 1] = (uint8_t)(yaw_motor_data & 0xFF); // LSB
        } else {
            tx_data2[(MOTOR_YAW_ID - 4 - 1) * 2] = (uint8_t)(yaw_motor_data >> 8); // MSB
            tx_data2[(MOTOR_YAW_ID - 4 - 1) * 2 + 1] = (uint8_t)(yaw_motor_data & 0xFF); // LSB
        }
        CAN_TxHeaderTypeDef can_tx_header = {
            //标准ID等待赋值
            .ExtId = 0, //扩展ID
            .IDE = CAN_ID_STD, //标准ID
            .RTR = CAN_RTR_DATA, //数据帧
            .DLC = 8, //数据长度
            .TransmitGlobalTime = DISABLE
        };
        uint32_t tx_mailbox;
        can_tx_header.StdId = 0x1FE;
        HAL_CAN_AddTxMessage(&hcan1, &can_tx_header, tx_data1, &tx_mailbox);
        can_tx_header.StdId = 0x2FE;
        HAL_CAN_AddTxMessage(&hcan1, &can_tx_header, tx_data2, &tx_mailbox);
        //不需要delay
        // osDelayUntil(ticks + TASK_DELAY_TIME_CAN_TX_TASK);
    }
}