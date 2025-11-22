//
// Created by HiAir on 2025/11/22.
// Sorry 因为作者太懒惰所以本库里面的所有SPI通信全是阻塞式。。。希望不要出问题
//

#include "bmi088.h"

#include "spi.h"

// chip selection
void BMI088_ACCEL_NS_L(void) {
    HAL_GPIO_WritePin(ACC_CS_GPIO_Port, ACC_CS_Pin, GPIO_PIN_RESET);
}

void BMI088_ACCEL_NS_H(void) {
    HAL_GPIO_WritePin(ACC_CS_GPIO_Port, ACC_CS_Pin, GPIO_PIN_SET);
}

void BMI088_GYRO_NS_L(void) {
    HAL_GPIO_WritePin(GYR_CS_GPIO_Port, GYR_CS_Pin, GPIO_PIN_RESET);
}

void BMI088_GYRO_NS_H(void) {
    HAL_GPIO_WritePin(GYR_CS_GPIO_Port, GYR_CS_Pin, GPIO_PIN_SET);
}

// bmi088 init
void bmi088_init(void);

// bmi088 read/write
void bmi088_write_byte(uint8_t tx_data) {
    HAL_SPI_Transmit(&hspi1, &tx_data, 1, HAL_MAX_DELAY);
}

void bmi088_read_byte(uint8_t* rx_data, uint8_t length) {
    HAL_SPI_Receive(&hspi1, rx_data, 1, HAL_MAX_DELAY);
}

void bmi088_write_reg(uint8_t reg, uint8_t data) {
    reg = reg & 0x7F; // 通过掩码设定写入/读取
    bmi088_write_byte(reg); // 指定寄存器
    bmi088_write_byte(data); // 写入数据
}

void bmi088_accel_write_single_reg(uint8_t reg, uint8_t data) {
    // 切换片选
    BMI088_GYRO_NS_H();
    BMI088_ACCEL_NS_L();
    // 写入寄存器
    bmi088_write_reg(reg, data);
    // 结束片选
    BMI088_ACCEL_NS_H();
}

void bmi088_accel_read_reg(uint8_t reg, uint8_t* rx_data, uint8_t length) {
    // 切换片选
    BMI088_GYRO_NS_H();
    BMI088_ACCEL_NS_L();
    // 接收数据
    uint8_t dumb_byte;
    bmi088_write_byte(reg | 0x80); // 通过掩码选择读取模式
    bmi088_read_byte(&dumb_byte, 1); // 丢弃 dumb byte
    bmi088_read_byte(rx_data, length); // 真正的数据
    // 结束片选
    BMI088_ACCEL_NS_H();
}

void bmi088_gyro_write_single_reg(uint8_t reg, uint8_t data) {
    BMI088_ACCEL_NS_H();
    BMI088_GYRO_NS_L();
    bmi088_write_reg(reg, data);
    BMI088_GYRO_NS_H();
}

void bmi088_gyro_read_reg(uint8_t reg, uint8_t* rx_data, uint8_t length) {
    // 切换片选
    BMI088_ACCEL_NS_H();
    BMI088_GYRO_NS_L();
    // 接收数据
    bmi088_write_byte(reg | 0x80); // 通过掩码选择读取模式
    bmi088_read_byte(rx_data, length); // 真正的数据
    // 结束片选
    BMI088_GYRO_NS_H();
}