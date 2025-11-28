//
// Created by HiAir on 2025/11/22.
//

#include "imu.h"

#include <cmath>

#include "main.h"
#include "mahony.h"
#include "bmi088.h"
#include "gimbal_settings.h"


IMU::IMU(const float& dt,
         const float& kg,
         const float& g_thres,
         const float R_imu[3][3],
         const float gyro_bias[3]):
    mahony_(Mahony(dt, kg, g_thres)),
    R_imu_{ { R_imu[0][0], R_imu[0][1], R_imu[0][2] },
            { R_imu[1][0], R_imu[1][1], R_imu[1][2] },
            { R_imu[2][0], R_imu[2][1], R_imu[2][2] } },
    gyro_bias_{ gyro_bias[0], gyro_bias[1], gyro_bias[2] } {}

void IMU::init() {
    //acc十次采样取平均作为初始值
    uint8_t raw_range;
    uint8_t rx_accel_data[6];
    float average_accel[3];
    for (int i = 0; i < 10; i++) {
        bmi088_accel_read_reg(0x41, &raw_range, 1);
        bmi088_accel_read_reg(0x12, rx_accel_data, 6);
        //calculate data 单位重力单位g
        average_accel[0] += (int16_t)(rx_accel_data[1] << 8 | rx_accel_data[0]) * 1000.f * 1.5f *
            pow(2, (raw_range + 1)) / 32768.f / 1000.f; //X
        average_accel[1] += (int16_t)(rx_accel_data[3] << 8 | rx_accel_data[2]) * 1000.f * 1.5f *
            pow(2, (raw_range + 1)) / 32768.f / 1000.f; //Y
        average_accel[2] += (int16_t)(rx_accel_data[5] << 8 | rx_accel_data[4]) * 1000.f * 1.5f *
            pow(2, (raw_range + 1)) / 32768.f / 1000.f; //Z
    }
    for (float& i: average_accel) {
        i /= 10.f;
    }
    euler_deg_.pitch = atan2(-average_accel[0],
                             sqrt(pow(average_accel[1], 2)
                                 + pow(average_accel[2], 2))) * 180.f / M_PI;
    euler_deg_.yaw = 0.0;
    euler_deg_.roll = 0.0;
}

void IMU::readSensor() {
    uint8_t raw_range;
    uint8_t rx_accel_data[6], rx_gyro_data[6];
    uint8_t rx_temp_data[2];

    // accelerator
    bmi088_accel_read_reg(0x41, &raw_range, 1);
    bmi088_accel_read_reg(0x12, rx_accel_data, 6);
    //calculate data 单位重力单位g
    raw_data_.accel[0] = (int16_t)(rx_accel_data[1] << 8 | rx_accel_data[0]) * 1000.f * 1.5f * pow(2, (raw_range + 1)) /
        32768.f / 1000.f; //X
    raw_data_.accel[1] = (int16_t)(rx_accel_data[3] << 8 | rx_accel_data[2]) * 1000.f * 1.5f * pow(2, (raw_range + 1)) /
        32768.f / 1000.f; //Y
    raw_data_.accel[2] = (int16_t)(rx_accel_data[5] << 8 | rx_accel_data[4]) * 1000.f * 1.5f * pow(2, (raw_range + 1)) /
        32768.f / 1000.f; //Z

    //gyro
    bmi088_gyro_read_reg(0x0F, &raw_range, 1);
    bmi088_gyro_read_reg(0x02, rx_gyro_data, 6);
    //calculate data 单位degree/s
    raw_data_.gyro[0] = (int16_t)(rx_gyro_data[1] << 8 | rx_gyro_data[0]) * 1000 / pow(2, (raw_range - 1)) / 32768.f;
    raw_data_.gyro[1] = (int16_t)(rx_gyro_data[3] << 8 | rx_gyro_data[2]) * 1000 / pow(2, (raw_range - 1)) / 32768.f;
    raw_data_.gyro[2] = (int16_t)(rx_gyro_data[5] << 8 | rx_gyro_data[4]) * 1000 / pow(2, (raw_range - 1)) / 32768.f;

    //temperature
    bmi088_accel_read_reg(0x22, rx_temp_data, 2);
    uint16_t temp_uint11;
    int16_t temp_int11;
    // 计算11位无符号温度值
    temp_uint11 = (rx_temp_data[0] * 8) + (rx_temp_data[1] / 32);
    // 转换为11位有符号温度值（2的补码）
    if (temp_uint11 > 1023) {
        temp_int11 = temp_uint11 - 2048;
    } else {
        temp_int11 = temp_uint11;
    }
    // 计算实际温度值（摄氏度）
    raw_data_.temp[0] = temp_int11 * 0.125f + 23.0f;
}

void IMU::update(void) {
    // 线性滤波解算
    float alpha = 0.95;
    float pitch_acc;
    float pitch_gyro, yaw_gyro;
    pitch_acc = atan2(-raw_data_.accel[0],
                      sqrt(pow(raw_data_.accel[1], 2)
                          + pow(raw_data_.accel[2], 2))) * 180.f / M_PI;
    pitch_gyro = -raw_data_.gyro[1]; // 负号是因为pitch轴定义正方向与角速度正方向相反
    euler_deg_.pitch = alpha * (euler_deg_.pitch + pitch_gyro * TASK_DELAY_TIME_IMU_TASK / 1000.0f) + (1 - alpha) *
        pitch_acc;
    yaw_gyro = raw_data_.gyro[2] / cos(euler_deg_.pitch / 180.0f * M_PI);
    euler_deg_.yaw += yaw_gyro * TASK_DELAY_TIME_IMU_TASK / 1000.0f; // 方向是上方看逆时针为正

    euler_rad_.pitch = euler_deg_.pitch / 180.0f * M_PI; // 同步更新
    euler_rad_.yaw = euler_deg_.yaw / 180.0f * M_PI;
    euler_rad_.roll = euler_deg_.roll / 180.0f * M_PI;
    // mahony 解算
}