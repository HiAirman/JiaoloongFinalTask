//
// Created by HiAir on 2025/11/18.
//

#ifndef JIAOLOONGFINALTASK_DATA_TYPES_H
#define JIAOLOONGFINALTASK_DATA_TYPES_H
#include "main.h"

// chx: -1 ~ +1 swx: -1 0 1
enum ControllerSwState {
    CONTROLLER_SW_DOWN = -1,
    CONTROLLER_SW_MID,
    CONTROLLER_SW_UP
};

typedef struct {
    float ch0, ch1, ch2, ch3;
    ControllerSwState sw1, sw2; // sw1 左侧 sw2 右侧
    uint32_t timestamp;
    uint32_t sequence = 0;
} controller_data_t;

typedef struct {
    float pitch = 0, roll = 0, yaw = 0;
    uint32_t timestamp;
    uint32_t sequence = 0;
} imu_data_t;

typedef struct {
    float pitch_motor_position, yaw_motor_position;
    float pitch_feedforward_intensity;
    uint32_t timestamp;
    uint32_t sequence = 0;
} motor_control_data_t;

typedef struct {
    // 电流单位 A
    float pitch_motor_current, yaw_motor_current;
    uint32_t timestamp;
    uint32_t sequence = 0;
} motor_output_data_t;

typedef struct {
    float pitch, roll, yaw;
    uint32_t timestamp;
    uint32_t sequence = 0;
} imu_output_data_t;

#endif //JIAOLOONGFINALTASK_DATA_TYPES_H