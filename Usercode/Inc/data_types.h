//
// Created by HiAir on 2025/11/18.
//

#ifndef JIAOLOONGFINALTASK_DATA_TYPES_H
#define JIAOLOONGFINALTASK_DATA_TYPES_H
#include <cstdint>

typedef struct {
    float ch1, ch2, ch3, ch4;
    uint8_t sw1, sw2;
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
    uint32_t timestamp;
    uint32_t sequence = 0;
} motor_output_data_t;

#endif //JIAOLOONGFINALTASK_DATA_TYPES_H