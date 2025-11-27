//
// Created by HiAir on 2025/11/1.
//

#include "user_tasks.h"

#include "cmsis_os2.h"

#include "gimbal_settings.h"
#include "data_types.h"
#include "control_task.h"
#include "can_tx_task.h"
#include "imu_task.h"
#include "motor_task.h"
#include "bmi088.h"
#include "can_rx.h"
#include "controller.h"

//Message Queues
//覆盖式队列：
//传输遥控器数据
osMessageQueueId_t dbus_to_control_queue_handle;
osMessageQueueAttr_t dbus_to_control_queue_attribute{
    .name = "dbus_to_control_queue",
};
//传输姿态数据
osMessageQueueId_t imu_to_control_queue_handle;
osMessageQueueAttr_t imu_to_control_queue_attribute{
    .name = "imu_to_control_queue",
};
//传输目标角度，前馈值
osMessageQueueId_t control_to_motor_queue_handle;
osMessageQueueAttr_t control_to_motor_queue_attribute{
    .name = "control_to_motor_queue",
};
//传输电机反馈值
osMessageQueueId_t can_rx_to_motor_queue_handle;
osMessageQueueAttr_t can_rx_to_motor_queue_attribute{
    .name = "can_rx_to_motor_queue",
};
//传输右摇杆flag
osMessageQueueId_t dbus_to_motor_queue_handle;
osMessageQueueAttr_t dbus_to_motor_queue_attribute{
    .name = "dbus_to_motor_queue",
};
//一般队列：
//传输电机输出值
osMessageQueueId_t motor_to_can_tx_queue_handle;
osMessageQueueAttr_t motor_to_can_tx_queue_attribute{
    .name = "motor_to_can_tx_queue",
};
//传输初始位置值
osMessageQueueId_t imu_to_motor_queue_handle;
osMessageQueueAttr_t imu_to_motor_queue_attribute{
    .name = "motor_to_can_tx_queue",
};

//Semaphore
// osSemaphoreId_t test_semaphore_handle;
// osSemaphoreAttr_t test_semaphore_attribute{
//     .name = "test_semaphore",
// };

//Event Flag
//osEventFlagsId_t test_event_flag_handle;
//osEventFlagsAttr_t test_event_flag_attribute{
//    .name = "test_event_flag"
//};

//tasks are moved into their files

void initialize_all_peripherals() {
    bmi088_init();
    controller.init();
    can_rx_init();
    //motor init
}

void user_task_init() {
    //Initialization初始化各组件
    initialize_all_peripherals();

    //MassageQueues & Semaphores & Eventflags开启各线程间通信
    dbus_to_control_queue_handle =
        osMessageQueueNew(1, sizeof(controller_data_t), &dbus_to_control_queue_attribute);
    imu_to_control_queue_handle =
        osMessageQueueNew(1, sizeof(imu_data_t), &imu_to_control_queue_attribute);
    control_to_motor_queue_handle =
        osMessageQueueNew(1, sizeof(motor_control_data_t), &control_to_motor_queue_attribute);
    can_rx_to_motor_queue_handle =
        osMessageQueueNew(1, sizeof(motor_feedback_data_t), &can_rx_to_motor_queue_attribute);
    dbus_to_motor_queue_handle =
        osMessageQueueNew(1, sizeof(int8_t), &dbus_to_motor_queue_attribute);
    motor_to_can_tx_queue_handle =
        osMessageQueueNew(1, sizeof(motor_output_data_t), &motor_to_can_tx_queue_attribute);
    imu_to_motor_queue_handle =
        osMessageQueueNew(1, sizeof(motor_initialize_data_t), &imu_to_control_queue_attribute);

    //Threads开启各线程
    //test
    control_task_handle = osThreadNew(control_task, nullptr, &control_task_attribute);
    imu_task_handle = osThreadNew(imu_task, nullptr, &imu_task_attribute);
    can_tx_task_handle = osThreadNew(can_tx_task, nullptr, &can_tx_task_attribute);
    motor_task_handle = osThreadNew(motor_task, nullptr, &motor_task_attribute);
    /*
    control_task_handle = osThreadNew(control_task, nullptr, &control_task_attribute);
    can_tx_task_handle = osThreadNew(can_tx_task, nullptr, &can_tx_task_attribute);
    imu_task_handle = osThreadNew(imu_task, nullptr, &imu_task_attribute);
    motor_task_handle = osThreadNew(motor_task, nullptr, &motor_task_attribute);
    */
}
