//
// Created by HiAir on 2025/11/1.
//

#include "user_tasks.h"

#include "cmsis_os2.h"

//Message Queues
// osMessageQueueId_t test_queue_handle;
// osMessageQueueAttr_t test_queue_attribute{
//     .name = "test_queue",
// };
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

//tasks
osThreadId_t control_task_handle;
osThreadAttr_t control_task_attribute{
    .name = "control_task",
    .stack_size = 128 * 4,
    .priority = osPriorityNormal,
};

[[noreturn]] void control_task(void*) {
    while (true) {
        //发送
        auto ticks = osKernelGetTickCount();

        osDelayUntil(ticks + 14);
    }
}

osThreadId_t can_tx_task_handle;
osThreadAttr_t can_tx_task_attribute{
    .name = "can_tx_task",
    .stack_size = 128 * 4,
    .priority = osPriorityNormal,
};

[[noreturn]] void can_tx_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();

        osDelayUntil(ticks + 1);
    }
}

osThreadId_t imu_task_handle;
osThreadAttr_t imu_task_attribute{
    .name = "imu_task",
    .stack_size = 128 * 4,
    .priority = osPriorityNormal,
};

[[noreturn]] void imu_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();

        osDelayUntil(ticks + 14);
    }
}

osThreadId_t motor_task_handle;
osThreadAttr_t motor_task_attribute{
    .name = "motor_task",
    .stack_size = 128 * 4,
    .priority = osPriorityNormal,
};

[[noreturn]] void motor_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();

        osDelayUntil(ticks + 1);
    }
}

void user_task_init() {
    // test_queue_handle = osMessageQueueNew(10, sizeof(uint32_t), &test_queue_attribute);

    // test_semaphore_handle = osSemaphoreNew(1, 0, &test_semaphore_attribute);

    //test_event_flag_handle = osEventFlagsNew(&test_event_flag_attribute);

    //test_task_handle = osThreadNew(test_task, nullptr, &test_task_attribute);

    //test2_task_handle = osThreadNew(test2_task, nullptr, &test2_task_attribute);

    //test3_task_handle = osThreadNew(test3_task, nullptr, &test3_task_attribute);

    //Initialization初始化各组件
    //MassageQueues & Semaphores & Eventflags开启各线程间通信
    dbus_to_control_queue_handle = osMessageQueueNew(1, 10, &dbus_to_control_queue_attribute);
    imu_to_control_queue_handle = osMessageQueueNew(1, 10, &imu_to_control_queue_attribute);
    control_to_motor_queue_handle = osMessageQueueNew(1, 10, &control_to_motor_queue_attribute);
    can_rx_to_motor_queue_handle = osMessageQueueNew(1, 10, &can_rx_to_motor_queue_attribute);
    dbus_to_motor_queue_handle = osMessageQueueNew(1, 10, &dbus_to_motor_queue_attribute);
    motor_to_can_tx_queue_handle = osMessageQueueNew(1, 10, &motor_to_can_tx_queue_attribute);

    //Threads开启各线程
    control_task_handle = osThreadNew(control_task, nullptr, &control_task_attribute);
    can_tx_task_handle = osThreadNew(can_tx_task, nullptr, &can_tx_task_attribute);
    imu_task_handle = osThreadNew(imu_task, nullptr, &imu_task_attribute);
    motor_task_handle = osThreadNew(motor_task, nullptr, &motor_task_attribute);
}
