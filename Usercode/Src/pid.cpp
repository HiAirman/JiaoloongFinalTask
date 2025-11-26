//
// Created by HiAir on 2025/11/26.
//

#include "pid.h"

#include <cmath>

PID::PID(float kp, float ki, float kd, float i_max, float out_max, float d_filter_k, float time):
    kp_(kp),
    ki_(ki),
    kd_(kd),
    i_max_(i_max),
    out_max_(out_max),
    d_filter_k_(d_filter_k),
    time_(time) {}

void PID::reset() {
    err_sum_ = 0.0f;
    //last_err_ = 0.0f;
    last_dout_ = 0.0f;
    is_last_err_updated_ = false;
}

float PID::calc(float ref, float fdb) {
    ref_ = ref;
    fdb_ = fdb;
    err_ = ref_ - fdb_;
    //err_ = 0.05 * err_ + 0.95 * last_err_;

    // 比例项
    pout_ = kp_ * err_;
    pout_ = 0.05f * pout_ + 0.95f * last_pout_;
    last_pout_ = pout_;

    // 积分项（带条件激活和限幅）
    if (fabs(err_) <= i_max_) {
        err_sum_ += err_;
        err_sum_ = fmax(fmin(err_sum_, 100000), -100000);
    }
    iout_ = ki_ * time_ * err_sum_;

    // 微分项（带滤波）
    if (is_last_err_updated_) {
        dout_ = (kd_ * (err_ - last_err_) / time_ * d_filter_k_ + last_dout_ * (1 - d_filter_k_));
    }
    // 更新历史值
    last_err_ = err_;
    is_last_err_updated_ = true;
    last_dout_ = dout_;
    //计算输出并限幅
    output_ = pout_ + iout_ + dout_;
    float limited_output = fmax(fmin(output_, out_max_), -out_max_);

    // 积分抗饱和
    if (output_ != limited_output) {
        err_sum_ -= err_; // 输出饱和时回退积分
    }

    return limited_output;
}