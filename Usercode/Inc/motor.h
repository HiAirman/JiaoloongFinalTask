//
// Created by HiAir on 2025/11/26.
//

#ifndef JIAOLOONGFINALTASK_MOTOR_H
#define JIAOLOONGFINALTASK_MOTOR_H
#include "pid.h"


class Motor {
public: // functions
    Motor():
        Motor(0, 0, 0, 0, 0, 0) {}

    Motor(float kp_position_pid,
          float ki_position_pid,
          float kd_position_pid,
          float kp_speed_pid,
          float ki_speed_pid,
          float kd_speed_pid);

    void set_target();
    float get_current();

public: //variables
private:
    PID position_pid, speed_pid;
};


#endif //JIAOLOONGFINALTASK_MOTOR_H