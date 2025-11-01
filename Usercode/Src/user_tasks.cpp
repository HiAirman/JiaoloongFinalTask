//
// Created by HiAir on 2025/11/1.
//

#include "user_tasks.h"

#include "cmsis_os2.h"

//Message Queues
osMessageQueueId_t test_queue_handle;
osMessageQueueAttr_t test_queue_attribute{
    .name = "test_queue",
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
        send++;
        osMessageQueuePut(test_queue_handle, &send, osPriorityNormal, 0);
        osDelayUntil(ticks + 1);
    }
}

osThreadId_t test2_task_handle;
osThreadAttr_t test2_task_attribute{
    .name = "test2_task",
    .stack_size = 128 * 4,
    .priority = osPriorityNormal,
};

uint32_t receive = 0;

[[noreturn]] void test2_task(void*) {
    while (true) {
        //接收
        osMessageQueueGet(test_queue_handle, &receive, nullptr, osWaitForever);
    }
}


void user_task_init() {
    test_queue_handle = osMessageQueueNew(10, sizeof(uint32_t), &test_queue_attribute);

    test_task_handle = osThreadNew(test_task, nullptr, &test_task_attribute);

    test2_task_handle = osThreadNew(test2_task, nullptr, &test2_task_attribute);
}
