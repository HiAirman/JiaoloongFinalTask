//
// Created by HiAir on 2025/11/18.
// include all the marcos for easy modifying
//

#ifndef JIAOLOONGFINALTASK_GIMBAL_SETTINGS_H
#define JIAOLOONGFINALTASK_GIMBAL_SETTINGS_H

//**task settings

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

//**control_task settings

#define CONTROL_PITCH_RANGE             1.0
#define CONTROL_PITCH_MAX               0.5
#define CONTROL_PITCH_MIN               - 0.5
#define CONTROL_YAW_RANGE               180

//**imu settings

#define IMU_CALCULATE_DELTA_TIME        0.001f
#define IMU_FUSION_COEFFICIENT          1
#define IMU_G_THRESHOLD                 1
#define IMU_GYRO_BIAS                   {0,0,0}

//**motor settings

//ID SHOULD IN RANGE 1 - 7
#define MOTOR_PITCH_ID                  4
#define MOTOR_YAW_ID                    1

//**pid settings

// PITCH
#define MOTOR_PITCH_POSITION_PID_KP     0.0
#define MOTOR_PITCH_POSITION_PID_KI     0.0
#define MOTOR_PITCH_POSITION_PID_KD     0.0
#define MOTOR_PITCH_POSITION_PID_I_MAX  0.0
#define MOTOR_PITCH_POSITION_PID_OUT_MAX 0.0

#define MOTOR_PITCH_SPEED_PID_KP        0.0
#define MOTOR_PITCH_SPEED_PID_KI        0.0
#define MOTOR_PITCH_SPEED_PID_KD        0.0
#define MOTOR_PITCH_SPEED_PID_I_MAX     0.0
#define MOTOR_PITCH_SPEED_PID_OUT_MAX   0.0

// YAW
#define MOTOR_YAW_POSITION_PID_KP       0.0
#define MOTOR_YAW_POSITION_PID_KI       0.0
#define MOTOR_YAW_POSITION_PID_KD       0.0
#define MOTOR_YAW_POSITION_PID_I_MAX    0.0
#define MOTOR_YAW_POSITION_PID_OUT_MAX  0.0

#define MOTOR_YAW_SPEED_PID_KP          0.0
#define MOTOR_YAW_SPEED_PID_KI          0.0
#define MOTOR_YAW_SPEED_PID_KD          0.0
#define MOTOR_YAW_SPEED_PID_I_MAX       0.0
#define MOTOR_YAW_SPEED_PID_OUT_MAX     0.0

#endif //JIAOLOONGFINALTASK_GIMBAL_SETTINGS_H