//
// Created by HiAir on 2025/11/24.
//

#ifndef JIAOLOONGFINALTASK_CONTROLLER_H
#define JIAOLOONGFINALTASK_CONTROLLER_H

#include "main.h"
#include "data_types.h"

//macros
//RC channel definition
#define RC_CH_VALUE_MIN ((uint16_t)364)
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)
//RC switch definition
#define RC_SW_UP ((uint16_t)1)
#define RC_SW_MID ((uint16_t)3)
#define RC_SW_DOWN ((uint16_t)2)

class Controller {
public:
    void init();
    //called in uart isr
    void dbus_isr(uint16_t size);

    bool is_controller_connected();

private:
    uint32_t sequence = 0;

    uint8_t rx_data_[32];
    controller_data_t controller_data_;

    uint8_t sw1_flag_ = -1;
};

extern Controller controller;


#endif //JIAOLOONGFINALTASK_CONTROLLER_H