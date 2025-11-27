//
// Created by HiAir on 2025/11/1.
//

#ifndef JIAOLOONGFINALTASK_USER_TASKS_H
#define JIAOLOONGFINALTASK_USER_TASKS_H
#include "cmsis_os2.h"

#ifdef __cplusplus
extern "C" {
#endif

void user_task_init();

#ifdef __cplusplus
}

extern osMessageQueueId_t dbus_to_control_queue_handle,
                          imu_to_control_queue_handle,
                          control_to_motor_queue_handle,
                          can_rx_to_motor_queue_handle,
                          dbus_to_motor_queue_handle,
                          motor_to_can_tx_queue_handle,
                          imu_to_motor_queue_handle;
#endif


#endif //JIAOLOONGFINALTASK_USER_TASKS_H