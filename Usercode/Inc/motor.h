//
// Created by HiAir on 2025/11/26.
//

#ifndef JIAOLOONGFINALTASK_MOTOR_H
#define JIAOLOONGFINALTASK_MOTOR_H
#include "pid.h"


class Motor {
public: // functions
    Motor():
        Motor(0, 0, 0, 0, 0, 0, 0, 0, 0, 0) {}

    Motor(float kp_position_pid,
          float ki_position_pid,
          float kd_position_pid,
          float i_max_position_pid,
          float out_max_position_pid,
          float kp_speed_pid,
          float ki_speed_pid,
          float kd_speed_pid,
          float i_max_speed_pid,
          float out_max_speed_pid);

    void init(float);

    //set target angle
    void set_target(float target_angle);
    //process feedback data
    void set_feedback(float feedback_angle, float feedback_speed);
    //get feedforward torque
    void set_feedforward_torque(float feedforward_torque);
    //check safety (no ways to reset but to reset the mcu)
    void check_safety(float current, float temperature);
    //output the current
    float get_current();

private: //functions
    void process_feedback();
    float calculate_feedforward_speed(float target_angle, float feedback_angle);

private:
    // PID object
    PID position_pid_, speed_pid_;

    // safety
    bool safety_flag_;

    // target
    float target_angle_, target_speed_;
    // feedback
    float raw_feedback_angle_;
    float last_raw_feedback_angle;
    float feedback_angle_, feedback_speed_;
    // feedforward
    float feedforward_torque_, feedforward_speed_;
    float feedforward_speed_k_;
    // output
    float output_current_;
};


#endif //JIAOLOONGFINALTASK_MOTOR_H