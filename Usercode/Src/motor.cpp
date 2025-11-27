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
    position_pid_ = PID(kp_position_pid,
                        ki_position_pid,
                        kd_position_pid,
                        0.0,
                        0.0);
    speed_pid_ = PID(kp_speed_pid,
                     ki_speed_pid,
                     kd_speed_pid,
                     0.0,
                     0.0);
}

void Motor::init(float init_feedback_angle) {
    // 给feedback_angle赋值
    feedback_angle_ = init_feedback_angle;
}


void Motor::set_target(float target_angle) {
    if (target_angle <= 360 && target_angle >= 0) {
        target_angle_ = target_angle;
    }
}

void Motor::set_feedback(float feedback_angle, float feedback_speed) {
    if (feedback_angle <= 360 && feedback_angle >= 0) {
        raw_feedback_angle_ = feedback_angle;
    }
    feedback_speed_ = feedback_speed;
    process_feedback();
}

void Motor::set_feedforward_torque(float feedforward_torque) {
    feedforward_torque_ = feedforward_torque;
}


float Motor::get_current() {
    // calculate target speed
    target_speed_ = position_pid_.calc(target_angle_, feedback_angle_)
        + calculate_feedforward_speed(target_angle_, feedback_angle_);
    // calculate output torque
    float output_torque;
    output_torque = speed_pid_.calc(target_speed_, feedback_speed_) + feedforward_torque_;
    if (output_torque >= 0) {
        return output_torque * 5.0f / 3.0f + 0.2; // current = torque * 5/3 +0.2
    }
    return output_torque * 5.0f / 3.0f - 0.2;
}

void Motor::process_feedback() {
    float delta_angle = raw_feedback_angle_ - last_raw_feedback_angle;
    if (delta_angle > 180.f) {
        delta_angle -= 360.f;
    } else if (delta_angle < -180.f) {
        delta_angle += 360.f;
    }
    feedback_angle_ += delta_angle;
}

float Motor::calculate_feedforward_speed(float target_angle, float feedback_angle) {
    return (target_angle - feedback_angle) * feedforward_speed_k_;
}


