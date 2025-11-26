//
// Created by HiAir on 2025/11/26.
//

#include "motor.h"

Motor::Motor(float kp_position_pid,
             float ki_position_pid,
             float kd_position_pid,
             float kp_speed_pid,
             float ki_speed_pid,
             float kd_speed_pid) {
    position_pid = PID(kp_position_pid,
                       ki_position_pid,
                       kd_position_pid,
                       0.0,
                       0.0);
    speed_pid = PID(kp_speed_pid,
                    ki_speed_pid,
                    kd_speed_pid,
                    0.0,
                    0.0);
}
