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

//Semaphore
// osSemaphoreId_t test_semaphore_handle;
// osSemaphoreAttr_t test_semaphore_attribute{
//     .name = "test_semaphore",
// };

//Event Flag
osEventFlagsId_t test_event_flag_handle;
osEventFlagsAttr_t test_event_flag_attribute{
    .name = "test_event_flag"
};

//tasks
osThreadId_t test_task_handle;
osThreadAttr_t test_task_attribute{
    .name = "test_task",
    .stack_size = 128 * 4,
    .priority = osPriorityNormal,
};

uint32_t send = 0;

[[noreturn]] void test_task(void*) {
    while (true) {
        //发送
        auto ticks = osKernelGetTickCount();
        //    send++;
        //    osMessageQueuePut(test_queue_handle, &send, osPriorityNormal, 0);
        // if (send++ % 5 == 0) {
        //     //预期：send增加5，receive增加1
        //     osSemaphoreRelease(test_semaphore_handle);
        // }
        if (send++ % 5 == 0) {
            osEventFlagsSet(test_event_flag_handle, (1u << 0));
        }
        if ((send - 1) % 5 == 0) {
            osEventFlagsClear(test_event_flag_handle, (1u << 0));
        }
        osDelayUntil(ticks + 1);
    }
}

osThreadId_t test2_task_handle;
osThreadAttr_t test2_task_attribute{
    .name = "test2_task",
    .stack_size = 128 * 4,
    .priority = osPriorityNormal,
};

[[noreturn]] void test2_task(void*) {
    while (true) {
        auto ticks = osKernelGetTickCount();
        if (send % 7 == 0) {
            osEventFlagsSet(test_event_flag_handle, (1u << 1));
        }
        if ((send - 1) % 7 == 0) {
            osEventFlagsClear(test_event_flag_handle, (1u << 1));
        }
        osDelayUntil(ticks + 1);
    }
}

osThreadId_t test3_task_handle;
osThreadAttr_t test3_task_attribute{
    .name = "test3_task",
    .stack_size = 128 * 4,
    .priority = osPriorityNormal,
};

uint32_t receive = 0;

[[noreturn]] void test3_task(void*) {
    while (true) {
        //接收
        //    osMessageQueueGet(test_queue_handle, &receive, nullptr, osWaitForever);
        // osSemaphoreAcquire(test_semaphore_handle, osWaitForever);
        osEventFlagsWait(test_event_flag_handle, (1u << 0) | (1u << 1), osFlagsWaitAll,osWaitForever);
        receive++;
    }
}

void user_task_init() {
    // test_queue_handle = osMessageQueueNew(10, sizeof(uint32_t), &test_queue_attribute);

    // test_semaphore_handle = osSemaphoreNew(1, 0, &test_semaphore_attribute);

    test_event_flag_handle = osEventFlagsNew(&test_event_flag_attribute);

    test_task_handle = osThreadNew(test_task, nullptr, &test_task_attribute);

    test2_task_handle = osThreadNew(test2_task, nullptr, &test2_task_attribute);

    test3_task_handle = osThreadNew(test3_task, nullptr, &test3_task_attribute);
}
