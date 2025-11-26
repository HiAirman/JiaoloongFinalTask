//
// Created by HiAir on 2025/11/18.
// include all the marcos for easy modifying
//

#ifndef JIAOLOONGFINALTASK_GIMBAL_SETTINGS_H
#define JIAOLOONGFINALTASK_GIMBAL_SETTINGS_H

//task settings

//STACK SIZE OF THE TASKS
#define TASK_STACK_SIZE_DEFAULT         4096

//PRIORITIES OF THE TASKS
#define TASK_PRIORITY_CONTROL_TASK      (osPriority_t)osPriorityNormal
#define TASK_PRIORITY_CAN_TX_TASK       (osPriority_t)osPriorityNormal
#define TASK_PRIORITY_IMU_TASK          (osPriority_t)osPriorityNormal
#define TASK_PRIORITY_MOTOR_TASK        (osPriority_t)osPriorityNormal

//DELAY TIME OF THE TASKS
#define TASK_DELAY_TIME_CONTROL_TASK    14
#define TASK_DELAY_TIME_CAN_TX_TASK     1
#define TASK_DELAY_TIME_IMU_TASK        14
#define TASK_DELAY_TIME_MOTOR_TASK      1

//Gimbal settings
#define IMU_CALCULATE_DELTA_TIME        0.001f
#define IMU_FUSION_COEFFICIENT          1
#define IMU_G_THRESHOLD                 1
#define IMU_GYRO_BIAS                   {0,0,0}


#endif //JIAOLOONGFINALTASK_GIMBAL_SETTINGS_H